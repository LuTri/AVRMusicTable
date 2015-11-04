#ifndef _WS2812_H_
#define _WS2812_H_

#include "globals.h"
#include <avr/io.h>

typedef struct { uint8_t g; uint8_t r; uint8_t b; } cRGB;

void ws2812_setleds(void);

cRGB leds[N_PACKS];

#endif /* WS2812_CONFIG_H_ */

