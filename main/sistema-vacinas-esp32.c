#include "nvs_flash.h"
#include "wifi.h"
#include "mqtt.h"
#include "temp.h"

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    wifi_init_sta();
    temp_init();
    xTaskCreate(temp_task, "temp_task", 2048, NULL, 5, NULL);
    mqtt_app_start();
}