#ifndef _MODES_H
#define _MODES_H

#include "ws2812/light_ws2812.h"
#include "hostcom/host.h"
#include "config.h"

void slave(void);
void mood(void);
void sound(void);

extern uint8_t mode;

#endif
