#ifndef TEMP_H
#define TEMP_H

#include "esp_err.h"

esp_err_t temp_init(int gpio);

esp_err_t temp_read(float *temperature,
                    float *humidity);

void temp_task(void *pvParameters);

#endif