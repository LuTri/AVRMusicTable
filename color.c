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

void HsvToRgb(HSV* hsv, RGB* rgb) {
	float H = hsv->h;
	float R, G, B;
	float V = hsv->V;
	float S = hsv->S;

	while (H < 0) { H += 360; };
	while (H >= 360) { H -= 360; };

	if (V <= 0) {
		R = G = B = 0;
	}
	else if (S <= 0) {
		R = G = B = V;
	}
	else {
		float hf = H / 60.0;
		uint8_t i = hf;
		float f = hf - i;
		float pv = V * (1 - S);
		float qv = V * (1 - S * f);
		float tv = V * (1 - S * (1 - f));
		switch (i) {

			// Red is the dominant color

			case 0:
				R = V;
				G = tv;
				B = pv;
				break;

			// Green is the dominant color

			case 1:
				R = qv;
				G = V;
				B = pv;
				break;
			case 2:
				R = pv;
				G = V;
				B = tv;
				break;

			// Blue is the dominant color

			case 3:
				R = pv;
				G = qv;
				B = V;
				break;
			case 4:
				R = tv;
				G = pv;
				B = V;
				break;

			// Red is the dominant color

			case 5:
				R = V;
				G = pv;
				B = qv;
				break;
			default:
				//LFATAL("i Value error in Pixel conversion, Value is %d", i);
				R = G = B = V; // Just pretend its black/white
				break;
		}
	}
	rgb->r = (uint8_t)(R * 255.0);
	rgb->g = (uint8_t)(G * 255.0);
	rgb->b = (uint8_t)(B * 255.0);
}
