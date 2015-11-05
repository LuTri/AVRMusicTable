#ifndef _MOODS_H
#define _MOODS_H

#include <avr/io.h>

uint8_t fill_mood(uint8_t starting_pos, float starting_h, float starting_v,
	uint8_t func_idx);

#endif
