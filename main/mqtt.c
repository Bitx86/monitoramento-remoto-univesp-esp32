#include <stdio.h>
#include "esp_log.h"
#include "mqtt_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "temp.h"

#define BROKER_URI     "mqtts://35.209.244.156:8883"
#define MQTT_USERNAME  "dkey-3afdb946"
#define MQTT_PASSWORD  "rPTYaZ1DfydTEJLrerRVGObBkKUBYHDo"
#define MQTT_CLIENT_ID "dkey-3afdb946"
#define TOPIC          "users/f91977b9-0715-4194-9893-59d28ee7382f/devices/dkey-3afdb946/data"

static const char *TAG = "MQTT";

extern const uint8_t cert_pem_start[] asm("_binary_cert_pem_start");
extern const uint8_t cert_pem_end[]   asm("_binary_cert_pem_end");

// Handle do cliente acessível internamente pela task
static esp_mqtt_client_handle_t mqtt_client = NULL;

// Task dedicada à publicação — evita bloquear o handler de eventos
static void mqtt_publish_task(void *pvParameters)
{
    float temperatura;
    char payload[64];

    while (1) {
        // Bloqueia até receber uma leitura de temp.c
        if (xQueueReceive(fila_temperatura, &temperatura, portMAX_DELAY) == pdTRUE) {
            snprintf(payload, sizeof(payload),
                     "{\"temperatura\": %.2f, \"status\": \"ok\"}", temperatura);

            esp_mqtt_client_publish(mqtt_client, TOPIC, payload, 0, 1, 0);
            ESP_LOGI(TAG, "Mensagem enviada: %s", payload);
        }
    }
}

static void mqtt_event_handler(void *arg, esp_event_base_t base,
                                int32_t event_id, void *event_data)
{
    switch (event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Conectado ao broker");
            // Inicia a task de publicação apenas após conexão confirmada
            xTaskCreate(mqtt_publish_task, "mqtt_publish_task", 4096, NULL, 5, NULL);
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "Desconectado do broker");
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "Erro MQTT");
            break;

        default:
            break;
    }
}

void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = BROKER_URI,
        .broker.verification.skip_cert_common_name_check = true,
        .broker.verification.certificate = (const char *)cert_pem_start,
        .credentials.username                = MQTT_USERNAME,
        .credentials.authentication.password = MQTT_PASSWORD,
        .credentials.client_id               = MQTT_CLIENT_ID,
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(mqtt_client);
}