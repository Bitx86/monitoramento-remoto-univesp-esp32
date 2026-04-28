#ifndef MQTT_H
#define MQTT_H

#include "esp_err.h"

void mqtt_app_start(void);
esp_err_t mqtt_send_temperature(float temperatura);

#endif // MQTT_H