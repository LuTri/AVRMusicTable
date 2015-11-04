#include "color.h"

#define RANGE 100.0
#define FIRST_THIRD RANGE/3
#define SECOND_THIRD RANGE - FIRST_THIRD

inline float cycle(float value, float step) {
	while (value < 0) value += step;
	while (value >= step) value -= step;

	return value;
}

void fast_hsi(float value, float intensity, RGB* rgb) {
	float r_p = 0, g_p = 0, b_p = 0;
	float tmp_val;
		
	value = cycle(value,RANGE);
	tmp_val = RANGE - cycle(value, RANGE);

	if (value >= 0 && value < FIRST_THIRD) {
		r_p = tmp_val * intensity;
		g_p = (RANGE - tmp_val) * intensity;
	} else if (value >= FIRST_THIRD && value < SECOND_THIRD) {
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
