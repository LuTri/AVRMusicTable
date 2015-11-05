#include "color.h"

#define RANGE 100.0
#define FIRST_THIRD RANGE/3
#define SECOND_THIRD RANGE - FIRST_THIRD

inline float cycle(float val, float step) {
	while (val < 0) val += step;
	while (val >= step) val -= step;

	return val;
}

void fast_hsi(float hue, float intensity, RGB* rgb) {
	float r_p = 0, g_p = 0, b_p = 0;
	float tmp_val;
		
	hue = cycle(hue,RANGE);
	tmp_val = RANGE - cycle(hue, RANGE);

	if (hue >= 0 && hue < FIRST_THIRD) {
		r_p = tmp_val * intensity;
		g_p = (RANGE - tmp_val) * intensity;
	} else if (hue >= FIRST_THIRD && hue < SECOND_THIRD) {
		g_p = tmp_val * intensity;
		b_p = (RANGE - tmp_val) * intensity;
	} else {
		b_p = tmp_val * intensity;
		r_p = (RANGE - tmp_val) * intensity;
	}

	rgb->r = 255 * (r_p / RANGE);
	rgb->b = 255 * (g_p / RANGE);
	rgb->g = 255 * (b_p / RANGE);
}
