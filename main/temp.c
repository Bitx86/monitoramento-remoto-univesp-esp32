#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/temperature_sensor.h"

static const char *TAG = "temp_interna";

void app_main(void)
{
    // Passo 1: Definir o handle e configurar o intervalo de medição.
    // A faixa escolhida (20~85°C) cobre temperaturas típicas de operação
    // do chip em ambiente controlado e sob carga.
    temperature_sensor_handle_t temp_handle = NULL;
    temperature_sensor_config_t temp_config = TEMPERATURE _SENSOR_CONFIG_DEFAULT(20, 85);

    // Passo 2: Instalar o driver do sensor de temperatura.
    ESP_ERROR_CHECK(temperature_sensor_install(&temp_config, &temp_handle));
    ESP_LOGI(TAG, "Sensor de temperatura interno instalado.");

    // Passo 3: Habilitar o sensor — o circuito interno passa a operar.
    ESP_ERROR_CHECK(temperature_sensor_enable(temp_handle));
    ESP_LOGI(TAG, "Sensor de temperatura habilitado. Iniciando leituras...");

    float temperatura_celsius = 0.0f;

    while (1) {
        // Passo 4: Realizar a leitura em graus Celsius.
        esp_err_t resultado = temperature_sensor_get_celsius(temp_handle, &temperatura_celsius);

        if (resultado == ESP_OK) {
            ESP_LOGI(TAG, "Temperatura interna do chip: %.2f °C", temperatura_celsius);
        } else {
            ESP_LOGE(TAG, "Falha na leitura do sensor (código: %s)", esp_err_to_name(resultado));
        }

        // Aguarda 2 segundos entre leituras.
        vTaskDelay(pdMS_TO_TICKS(2000));
    }

    // Código de limpeza — em aplicações reais com ciclo de vida definido:
    // ESP_ERROR_CHECK(temperature_sensor_disable(temp_handle));
    // ESP_ERROR_CHECK(temperature_sensor_uninstall(temp_handle));
}