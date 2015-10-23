#ifndef _CONFIG_H
#define _CONFIG_H

#include <avr/io.h>

#define CMD_SOUNDTOLIGHT	0x01
#define CMD_SLAVEMODE		0x02
#define CMD_MOOD		0x03

#define STAT_LED PB5

#define N_COLS 14
#define N_ROWS 8

#endif
