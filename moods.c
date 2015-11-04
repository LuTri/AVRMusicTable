#include "moods.h"
#include "modes.h"
#include "color.h"
#include "ws2812.h"
#include "trans.h"

extern cRGB leds[N_PACKS];

uint16_t cycle = 0;

void mood1(float h, float v, float distance, RGB* vals) {
	fast_hsi(h, v, vals);
}

void mood2(float h, float v, float distance, RGB* vals) {
	fast_hsi(h, v, vals);
}

void mood3(float h, float v, float distance, RGB* vals) {
	fast_hsi(h, v, vals);
}

void mood4(float h, float v, float distance, RGB* vals) {
	fast_hsi(h, v, vals);
}

void (*funcs[])(float, float, float, RGB*) = {mood1, mood2, mood3, mood4};

void fill_mood(uint8_t starting_pos, float starting_h, float starting_v,
	uint8_t func) {
	uint8_t idx;
	float distance;
	RGB rgb;

	for (idx = 0; idx < N_PACKS; idx++) {
		distance = distance_snake(idx, starting_pos);
		(*funcs[func])(starting_h, starting_v, distance, &rgb);

		leds[idx].r = rgb.r;
		leds[idx].g = rgb.g;
		leds[idx].b = rgb.b;
	}
}
