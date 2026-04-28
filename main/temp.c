#include "temp.h"
#include <dht.h>

#define SENSOR_TYPE DHT_TYPE_AM2301

static int dht_gpio;

esp_err_t temp_init(int gpio)
{
    dht_gpio = gpio;
    return ESP_OK;
}

esp_err_t temp_read(float *temperature,
                    float *humidity)
{
    return dht_read_float_data(
        SENSOR_TYPE,
        dht_gpio,
        humidity,
        temperature);
}