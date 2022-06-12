#ifndef _WS2812_H_
#define _WS2812_H_

#include "globals.h"

#ifndef UNITTEST
#include <avr/io.h>
#else
#include "unittest.h"
#endif

#define T_CLOCK (1.0 / F_CPU)
#define SEND_CYCLES_BIT 20.0

#define C_FULL_SEND (SEND_CYCLES_BIT * N_LEDS * 8.0)
#define T_FULL_SEND (C_FULL_SEND * T_CLOCK)
/* approximately 3.36ms */

#ifndef WS2812_PAUSE_INTERRUPTS
#define WS2812_PAUSE_INTERRUPTS 0
#endif

typedef struct { uint8_t g; uint8_t r; uint8_t b; } cRGB;

uint8_t ws2812_setleds(void);

cRGB leds[N_PACKS];

void set_led(uint8_t idx, uint8_t r, uint8_t g, uint8_t b);

#endif /* WS2812_CONFIG_H_ */

