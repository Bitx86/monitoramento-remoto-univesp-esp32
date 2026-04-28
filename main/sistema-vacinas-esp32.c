#include "nvs_flash.h"
#include "wifi.h"
#include "mqtt.h"
#include "temp.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define WIFI_LED GPIO_NUM_22
#define MQTT_LED GPIO_NUM_23
#define DELAY_MQTT 2000

void app_main(void)
{
    gpio_reset_pin(WIFI_LED);
    gpio_reset_pin(MQTT_LED);

    gpio_set_direction(WIFI_LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(MQTT_LED, GPIO_MODE_OUTPUT);

    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);
    wifi_init_sta();
    temp_init(33);
    mqtt_app_start();

    if (wifi_is_connected())
        gpio_set_level(WIFI_LED, 1);
    else
        gpio_set_level(WIFI_LED, 0);

    while (1)
    {
        float temperatura, umidade;

        if (temp_read(&temperatura, &umidade) == ESP_OK)
        {
            if (mqtt_send_temperature(temperatura) == ESP_OK){
                gpio_set_level(MQTT_LED, 1);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(DELAY_MQTT));
        gpio_set_level(MQTT_LED, 0);
    }
}