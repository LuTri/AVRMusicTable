#ifndef _COLOR_H
#define _COLOR_H

#ifndef UNITTEST
#include <avr/io.h>
#else /* UNITTEST */
#include "unittest.h"
#endif /* UNITTEST */

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

#define HSI_RANGE       360.f

#define HSI_CELL_RANGE  120.f

#define HSI_LIMIT_RED   120.f
#define HSI_LIMIT_GREEN 240.f

#ifdef UNITTEST

#define HSI_MAX_GRANULAR_ERROR .0314f

typedef struct {
    uint8_t _do;
    float hue;
    float intensity;
    RGB rgb;
} TESTDATA;

int print_expected_rgb(TESTDATA* expected, RGB* actual, float granularity, const char* file, const int line);

#define CONVERSION_TEST_FORMAT \
    "%s:%i Expected: (R:%3d, G:%3d, B:%3d) - Actual: (R:%3d, G:%3d, B:%3d) [From input hue: %7.4f, intensity: %.4f]  -(relative error= %f)\n"

#define CONVERSION_EQUALS_RELATIVE(expected, actual) \
    print_expected_rgb(expected, actual, HSI_MAX_GRANULAR_ERROR, __FILE__, __LINE__)

#define RGB_EQUAL(expected, actual) \
    ((expected.r == actual->r) && (expected.g == actual->g) && (expected.b == actual->b))

#endif /* ifdef UNITTEST */

/** @brief  Convert Hue/Intensity into RGB.
 *
 *  Instead of calling costly -lm functions, do a rather fast apporximation
 *  instead.
 *  See https://en.wikipedia.org/wiki/HSL_and_HSV#HSI_to_RGB for a detailed
 *  description.
 *  @param  hue the hue to convert.
 *  @param  intensity_real the intensity to convert as a real number
            ([0,1] -> [0,255]).
 *  @param  rgb pointer a struct RGB where the results are written to.
 */

void fast_hsi(float hue, float intensity_percent, RGB* rgb);

#endif
