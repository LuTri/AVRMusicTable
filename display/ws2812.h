#ifndef _WS2812_H_
#define _WS2812_H_

#include "globals.h"

#ifndef UNITTEST
#include <avr/io.h>
#else
#include "unittest.h"
#endif

#ifndef WS2812_PAUSE_INTERRUPTS
#define WS2812_PAUSE_INTERRUPTS 0
#endif

typedef struct { uint8_t g; uint8_t r; uint8_t b; } cRGB;

void ws2812_setleds(void);
void ws2812_pwm_out(void);

cRGB leds[N_PACKS];

void set_led(uint8_t idx, uint8_t r, uint8_t g, uint8_t b);

#endif /* WS2812_CONFIG_H_ */

