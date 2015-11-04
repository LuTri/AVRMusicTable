#ifndef _MOODS_H
#define _MOODS_H

#include <avr/io.h>

void fill_mood(uint8_t starting_pos, float starting_h, float starting_v,
	uint8_t func);

#endif
