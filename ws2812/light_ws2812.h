#ifndef LIGHT_WS2812_H_
#define LIGHT_WS2812_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include "ws2812_config.h"

typedef struct { uint8_t g; uint8_t r; uint8_t b; } cRGB;

void ws2812_setleds(void);

cRGB leds[N_PACKS];

#define CONCAT(a, b)            a ## b
#define CONCAT_EXP(a, b)   CONCAT(a, b)

#define ws2812_PORTREG  CONCAT_EXP(PORT,ws2812_port)
#define ws2812_DDRREG   CONCAT_EXP(DDR,ws2812_port)

#endif /* LIGHT_WS2812_H_ */

