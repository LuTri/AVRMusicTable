#include "color.h"
#ifdef UNITTEST
#include <stdio.h>
#endif

inline float _cycle(float val, float step) {
	while (val < 0) val += step;
	while (val >= step) val -= step;

	return val;
}

void fast_hsi(float hue, float intensity_percent, RGB* rgb) {
	float cell_position = 0;

	hue = _cycle(hue, HSI_RANGE);

	cell_position = (_cycle(HSI_RANGE - hue, HSI_CELL_RANGE) /
	                 HSI_CELL_RANGE);

	if (cell_position == 0) cell_position = 1;

	if (hue < HSI_LIMIT_RED) {
	    rgb->r = (uint8_t)(cell_position * intensity_percent * 255);
	    rgb->g = (uint8_t)((1 - cell_position) * intensity_percent * 255);
	    rgb->b = 0;
	} else if (hue < HSI_LIMIT_GREEN) {
	    rgb->r = 0;
		rgb->g = (uint8_t)(cell_position * intensity_percent * 255);
		rgb->b = (uint8_t)((1 - cell_position) * intensity_percent * 255);
	} else {
		rgb->b = (uint8_t)(cell_position * intensity_percent * 255);
		rgb->r = (uint8_t)((1 - cell_position) * intensity_percent * 255);
	    rgb->g = 0;
	}
}

#ifdef UNITTEST
int print_expected_rgb(TESTDATA* expected, RGB* actual, float granularity,
                       const char* file, const int line) {
    float rel_error = (fabsf(expected->rgb.r - actual->r) +
                       fabsf(expected->rgb.g - actual->g) +
                       fabsf(expected->rgb.b - actual->b));
    rel_error = 1 - fabsf(1 - (rel_error / (0xff * 6)));
    int _equals = (RGB_EQUAL(expected->rgb, actual) || (rel_error < granularity));
    if (!(_equals))
        printf(CONVERSION_TEST_FORMAT,
               file, line,
               expected->rgb.r, expected->rgb.g, expected->rgb.b,
               actual->r, actual->g, actual->b,
               expected->hue, expected->intensity,
               rel_error);

    return _equals;
}

void test_fast_hsi(void) {
	RGB _result;
	TESTDATA expected[] = {
        { ._do = 1, .hue =    0.0, .intensity = 1.0, .rgb = { .r = 255, .g =   0, .b =  0 } },
        { ._do = 1, .hue =   24.0, .intensity = 1.0, .rgb = { .r = 180, .g =  74, .b =  0 } },
        { ._do = 1, .hue =   48.0, .intensity = 1.0, .rgb = { .r = 143, .g = 111, .b =  0 } },
        { ._do = 1, .hue =   72.0, .intensity = 1.0, .rgb = { .r = 111, .g = 143, .b =  0 } },
        { ._do = 1, .hue =   96.0, .intensity = 1.0, .rgb = { .r =  74, .g = 180, .b =  0 } },

        { ._do = 1, .hue =  120.0, .intensity = 1.0, .rgb = { .r =   0, .g = 255, .b =  0 } },
        { ._do = 1, .hue =  144.0, .intensity = 1.0, .rgb = { .r =   0, .g = 180, .b = 74 } },
        { ._do = 1, .hue =  168.0, .intensity = 1.0, .rgb = { .r =   0, .g = 143, .b =111 } },
        { ._do = 1, .hue =  192.0, .intensity = 1.0, .rgb = { .r =   0, .g = 111, .b =143 } },
        { ._do = 1, .hue =  216.0, .intensity = 1.0, .rgb = { .r =   0, .g =  74, .b =180 } },

        { ._do = 1, .hue =  240.0, .intensity = 1.0, .rgb = { .r =   0, .g =   0, .b =255 } },
        { ._do = 1, .hue =  264.0, .intensity = 1.0, .rgb = { .r =  74, .g =   0, .b =180 } },
        { ._do = 1, .hue =  288.0, .intensity = 1.0, .rgb = { .r = 111, .g =   0, .b =143 } },
        { ._do = 1, .hue =  312.0, .intensity = 1.0, .rgb = { .r = 143, .g =   0, .b =111 } },
        { ._do = 1, .hue =  336.0, .intensity = 1.0, .rgb = { .r = 180, .g =   0, .b = 74 } },

        { ._do = 1, .hue =    0.0, .intensity = .5, .rgb = { .r = 127, .g =   0, .b =  0 } },
        { ._do = 1, .hue =   24.0, .intensity = .5, .rgb = { .r =  90, .g =  37, .b =  0 } },
        { ._do = 1, .hue =   48.0, .intensity = .5, .rgb = { .r =  71, .g =  55, .b =  0 } },
        { ._do = 1, .hue =   72.0, .intensity = .5, .rgb = { .r =  55, .g =  71, .b =  0 } },
        { ._do = 1, .hue =   96.0, .intensity = .5, .rgb = { .r =  37, .g =  90, .b =  0 } },

        { ._do = 1, .hue =  120.0, .intensity = .5, .rgb = { .r =   0, .g = 127, .b =  0 } },
        { ._do = 1, .hue =  144.0, .intensity = .5, .rgb = { .r =   0, .g =  90, .b = 37 } },
        { ._do = 1, .hue =  168.0, .intensity = .5, .rgb = { .r =   0, .g =  71, .b = 55 } },
        { ._do = 1, .hue =  192.0, .intensity = .5, .rgb = { .r =   0, .g =  55, .b = 71 } },
        { ._do = 1, .hue =  216.0, .intensity = .5, .rgb = { .r =   0, .g =  37, .b = 90 } },

        { ._do = 1, .hue =  240.0, .intensity = .5, .rgb = { .r =   0, .g =   0, .b =127 } },
        { ._do = 1, .hue =  264.0, .intensity = .5, .rgb = { .r =  37, .g =   0, .b = 90 } },
        { ._do = 1, .hue =  288.0, .intensity = .5, .rgb = { .r =  55, .g =   0, .b = 71 } },
        { ._do = 1, .hue =  312.0, .intensity = .5, .rgb = { .r =  71, .g =   0, .b = 55 } },
        { ._do = 1, .hue =  336.0, .intensity = .5, .rgb = { .r =  90, .g =   0, .b = 37 } },

        { ._do = 1, .hue =  20.5556, .intensity = 0.2430, .rgb = { .r =  51, .g =  10, .b =   0 } },
        { ._do = 1, .hue =  70.2778, .intensity = 0.4820, .rgb = { .r =  50, .g =  71, .b =   0 } },
        { ._do = 1, .hue =  24.8889, .intensity = 0.3680, .rgb = { .r =  74, .g =  19, .b =   0 } },
        { ._do = 1, .hue = 234.3611, .intensity = 0.8030, .rgb = { .r =   0, .g =   9, .b = 195 } },
        { ._do = 1, .hue =  68.8333, .intensity = 0.9750, .rgb = { .r = 106, .g = 142, .b =   0 } },
        { ._do = 1, .hue =  96.8333, .intensity = 0.6570, .rgb = { .r =  32, .g = 135, .b =   0 } },
        { ._do = 1, .hue =  77.3056, .intensity = 0.0380, .rgb = { .r =   3, .g =   6, .b =   0 } },
        { ._do = 1, .hue =  49.8611, .intensity = 0.3560, .rgb = { .r =  53, .g =  37, .b =   0 } },
        { ._do = 1, .hue =  14.3333, .intensity = 0.3090, .rgb = { .r =  69, .g =   9, .b =   0 } },
        { ._do = 1, .hue =  72.3333, .intensity = 0.7140, .rgb = { .r =  72, .g = 109, .b =   0 } },
        { ._do = 1, .hue = 176.1111, .intensity = 0.1450, .rgb = { .r =  13, .g =   0, .b =  23 } },
        { ._do = 1, .hue =  63.5278, .intensity = 0.2260, .rgb = { .r =  27, .g =  30, .b =   0 } },
        { ._do = 1, .hue =  49.3611, .intensity = 0.8560, .rgb = { .r = 128, .g =  89, .b =   0 } },
        { ._do = 1, .hue =  73.5000, .intensity = 0.8490, .rgb = { .r =  83, .g = 132, .b =   0 } },
        { ._do = 1, .hue =  52.7500, .intensity = 0.6440, .rgb = { .r =  92, .g =  72, .b =   0 } },

		{ ._do = 0 },
	};
	TESTDATA* next_data = expected;

	while (next_data->_do) {
        fast_hsi(next_data->hue, next_data->intensity, &_result);

		CU_ASSERT(CONVERSION_EQUALS_RELATIVE(next_data, &_result));
		next_data++;
	}
}

char SUITE_NAME[] = "color.c - Color code conversion";

T_FUNC_DESCRIPTOR ALL_TEST_FUNCS[] = {
    { .fnc = &test_fast_hsi, .label = "test of fast_hsi()" },
};
#endif