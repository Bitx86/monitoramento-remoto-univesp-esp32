#ifndef TEMP_H
#define TEMP_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Fila acessível por mqtt.c
extern QueueHandle_t fila_temperatura;

void temp_init(void);
void temp_task(void *pvParameters);

#endif