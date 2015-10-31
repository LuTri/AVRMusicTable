#ifndef _MOODS_H
#define _MOODS_H

#include <avr/io.h>
#include "config.h"

typedef struct {
	float h;
	uint8_t v;
} HV;

extern HV mood_arr[N_COLS][N_ROWS];

void fill_mood(uint16_t starting_pos, float starting_h, float starting_v,
	uint8_t func);

void mood1(void);
void mood2(void);
void mood3(void);
void mood4(void);

#endif
