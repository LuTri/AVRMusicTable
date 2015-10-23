#ifndef _COLOR_H
#define _COLOR_H

#include <avr/io.h>

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} RGB;

typedef struct {
	float h;
	float S;
	float V;
} HSV;

void fast_hsi(float value, float intensity, RGB* rgb);

#endif
