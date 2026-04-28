#include <stdio.h>
#include "esp_log.h"
#include "mqtt_client.h"
#include "driver/gpio.h"

#define BROKER_URI     CONFIG_MQTT_BROKER_URI
#define MQTT_USERNAME  CONFIG_MQTT_USERNAME
#define MQTT_PASSWORD  CONFIG_MQTT_PASSWORD
#define MQTT_CLIENT_ID CONFIG_MQTT_CLIENT_ID
#define TOPIC          CONFIG_MQTT_TOPIC

#define LED_PIN GPIO_NUM_23

static const char *TAG = "MQTT";

extern const uint8_t cert_pem_start[] asm("_binary_cert_pem_start");
extern const uint8_t cert_pem_end[]   asm("_binary_cert_pem_end");

static esp_mqtt_client_handle_t mqtt_client = NULL;

static void mqtt_event_handler(
    void *arg,
    esp_event_base_t base,
    int32_t event_id,
    void *event_data)
{
    switch (event_id) {

        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Conectado ao broker");
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
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);

    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = BROKER_URI,
        .broker.verification.skip_cert_common_name_check = true,
        .broker.verification.certificate =
            (const char *)cert_pem_start,

        .credentials.username       = MQTT_USERNAME,
        .credentials.authentication.password = MQTT_PASSWORD,
        .credentials.client_id      = MQTT_CLIENT_ID,
    };

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(
        mqtt_client,
        ESP_EVENT_ANY_ID,
        mqtt_event_handler,
        NULL);

    esp_mqtt_client_start(mqtt_client);
}

esp_err_t mqtt_send_temperature(float temperatura)
{
    if (mqtt_client == NULL)
        return ESP_ERR_INVALID_STATE;

    char payload[64];

    snprintf(
        payload,
        sizeof(payload),
        "{\"temperatura\": %.2f, \"status\": \"ok\"}",
        temperatura);

    gpio_set_level(LED_PIN, 1);

    int msg_id = esp_mqtt_client_publish(
        mqtt_client,
        TOPIC,
        payload,
        0,
        1,
        0);

    gpio_set_level(LED_PIN, 0);

    if (msg_id < 0)
        return ESP_FAIL;

    ESP_LOGI(TAG, "Mensagem enviada: %s", payload);

    return ESP_OK;
}