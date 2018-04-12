#include "moods.h"
#include "modes.h"
#include "color.h"
#include "ws2812.h"
#include "trans.h"
#include <util/delay.h>

extern cRGB leds[N_PACKS];

uint8_t cycle = 0;

typedef struct {
	float hue;
	float value;
	float distance;
} HVd;

void mood1(HVd* hvd, RGB* vals) {
	float t_h, t_v;

	t_h = (hvd->hue + cycle * 2) * (hvd->distance / MAX_DISTANCE);
	t_v = 1.0 * (hvd->distance / MAX_DISTANCE);

	fast_hsi(t_h, t_v, vals);
	
}

void mood2(HVd* hvd, RGB* vals) {
}

void mood3(HVd* hvd, RGB* vals) {
}

void mood4(HVd* hvd, RGB* vals) {
}

void (*funcs[])(HVd*, RGB*) = {mood1, mood2, mood3, mood4};

uint8_t pos = 0;

uint8_t fill_mood(uint8_t starting_pos, float starting_h, float starting_v,
	uint8_t func_idx) {
	uint8_t idx;
	void (*func)(HVd*, RGB*) = funcs[func_idx];

	RGB rgb;
	HVd hvd;

	hvd.hue = starting_h;
	hvd.value = starting_v;

	for (idx = 0; idx < N_PACKS; idx++) {
		hvd.distance = distance_snake(starting_pos, idx);
		(*func)(&hvd, &rgb);

		leds[idx].r = rgb.r;
		leds[idx].g = rgb.g;
		leds[idx].b = rgb.b;	
	}
	cycle++;

	if (cycle == 200) {
		cycle = 0;
	}

	return 1;
}
