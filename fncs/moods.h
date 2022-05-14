#ifndef _MOODS_H
#define _MOODS_H

#ifndef UNITTEST
#include <avr/io.h>
#else
#include "unittest.h"
#endif

uint8_t fill_mood(uint8_t center_y, uint8_t center_x, float starting_h, float starting_v,
	uint8_t func_idx);

#endif
