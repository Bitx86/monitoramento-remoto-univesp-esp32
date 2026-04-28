#ifndef WIFI_H
#define WIFI_H

#include <stdbool.h>

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

void wifi_init_sta(void);

bool wifi_is_connected(void);

#endif