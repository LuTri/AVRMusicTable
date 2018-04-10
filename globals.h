#ifndef _GLOBALS_H
#define _GLOBALS_H

#define CMD_SOUNDTOLIGHT	0x01
#define CMD_SLAVEMODE		0x02
#define CMD_MOOD			0x03
#define CMD_WHITE			0x04
#define CMD_OFF				0x05

#define STAT_LED PB5

#define N_COLS 14
#define N_ROWS 8

#define ws2812_port B     // Data port 
#define ws2812_pin  0b00010000     // Data out pin -> D12 / PB4

#define CONCAT(a, b)            a ## b
#define CONCAT_EXP(a, b)   CONCAT(a, b)

#define ws2812_PORTREG  CONCAT_EXP(PORT,ws2812_port)
#define ws2812_DDRREG   CONCAT_EXP(DDR,ws2812_port)
#define N_PACKS N_ROWS * N_COLS
#define N_LEDS (N_PACKS * 3)


#endif
