#include "color.h"

inline float cycle(float value, float step) {
	while (value < 0) value += step;
	while (value >= step) value -= step;

	return value;
}

void fast_hsi(float value, float intensity, RGB* rgb) {
	float r_p = 0, g_p = 0, b_p = 0;
	float tmp_val;
		
	value = cycle(value,900.0);
	tmp_val = 100.0 - (
		cycle(value, 300.0) / 300.0 * 100.0);

	if (value >= 0 && value < 300.0) {
		r_p = tmp_val * intensity;
		g_p = (100 - tmp_val) * intensity;
	} else if (value >= 300.0 && value < 600) {
		g_p = tmp_val * intensity;
		b_p = (100 - tmp_val) * intensity;
	} else {
		b_p = tmp_val * intensity;
		r_p = (100 - tmp_val) * intensity;
	}

	rgb->r = 255 * (r_p / 100);
	rgb->b = 255 * (g_p / 100);
	rgb->g = 255 * (b_p / 100);
}
