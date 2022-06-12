#ifndef _GLOBALS_H
#define _GLOBALS_H

#define N_COLS 14
#define N_ROWS 8

#define WS2812_SPLIT 4
#define ws2812_port B     // Data port
#define ws2812_pin  0b00011110     // Data out pin -> D12-D9 / PB4-PB1

#define CONCAT(a, b)            a ## b
#define CONCAT_EXP(a, b)   CONCAT(a, b)

#ifndef UNITTEST
#define STAT_LED PB5

#define ws2812_PORTREG  CONCAT_EXP(PORT,ws2812_port)
#define ws2812_DDRREG   CONCAT_EXP(DDR,ws2812_port)

#else  /* ifndef UNITTEST */
#include "unittest.h"

extern uint8_t ws2812_PORTREG;
extern uint8_t ws2812_DDRREG;

#endif /* ifndef UNITTEST */

#define N_PACKS N_ROWS * N_COLS
#define N_LEDS (N_PACKS * 3)

#define EXPLICIT(a,b) ((a ^ b) == 0)


#endif
