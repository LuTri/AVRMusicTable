#include "moods.h"
#include "modes.h"
#include "color.h"
#include "ws2812.h"
#include "trans.h"

extern cRGB leds[N_PACKS];

typedef struct {
	float hue;
	float value;
	float distance;
} HVd;

void mood1(uint8_t idx, HVd* hvd, RGB* vals) {
}

void mood2(uint8_t idx, HVd* hvd, RGB* vals) {
}

void mood3(uint8_t idx, HVd* hvd, RGB* vals) {
}

void mood4(uint8_t idx, HVd* hvd, RGB* vals) {
}

void (*funcs[])(uint8_t, HVd*, RGB*) = {mood1, mood2, mood3, mood4};

uint8_t fill_mood(uint8_t starting_pos, float starting_h, float starting_v,
	uint8_t func) {
	uint8_t idx;
	RGB rgb;
	HVd hvd;

	return 0;
}
