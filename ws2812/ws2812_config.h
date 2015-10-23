/*
 * light_ws2812_config.h
 *
 * Created: 18.01.2014 09:58:15
 *
 * User Configuration file for the light_ws2812_lib
 *
 */ 


#ifndef WS2812_CONFIG_H_
#define WS2812_CONFIG_H_

#include "config.h"

///////////////////////////////////////////////////////////////////////
// Define I/O pin
///////////////////////////////////////////////////////////////////////

#define ws2812_port B     // Data port 
#define ws2812_pin  0b00010000     // Data out pin

#define N_PACKS N_ROWS * N_COLS
#define N_LEDS (N_PACKS * 3)

#endif /* WS2812_CONFIG_H_ */
