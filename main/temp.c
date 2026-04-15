#include "temp.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "TEMP";

QueueHandle_t fila_temperatura = NULL;

static adc_oneshot_unit_handle_t adc_handle;

void temp_init(void)
{
    fila_temperatura = xQueueCreate(5, sizeof(float));
    if (fila_temperatura == NULL) {
        ESP_LOGE(TAG, "Falha ao criar fila de temperatura");
    }

    adc_oneshot_unit_init_cfg_t init_cfg = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_cfg, &adc_handle));

    adc_oneshot_chan_cfg_t chan_cfg = {
        .atten    = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL_6, &chan_cfg));
}

void temp_task(void *pvParameters)
{
    int raw = 0;
    float mv, temperatura;

    while (1) {
        ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHANNEL_6, &raw));

        mv          = (raw * 3300.0f) / 4095.0f;
        temperatura = mv / 10.0f;

        ESP_LOGI(TAG, "Raw: %d | %.1f mV | %.2f°C", raw, mv, temperatura);

        if (xQueueSend(fila_temperatura, &temperatura, 0) != pdTRUE) {
            ESP_LOGW(TAG, "Fila cheia, leitura descartada");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}