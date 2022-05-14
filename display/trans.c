#include <math.h>
#include <stdio.h>

#ifndef UNITTEST
#include <avr/io.h>
#else
#include "unittest.h"
#endif

#include "trans.h"
#include "globals.h"


/******************************************************************************
 *                                                                            *
 * Functions to compensate the "snakish" pattern of the leds:                 *
 *  0 1 2 3                                                                   *
 *  7 6 5 4                                                                   *
 *  8 9 ...                                                                   *
 *                                                                            *
 *****************************************************************************/

uint8_t coord_to_snakish(uint8_t x, uint8_t y) {
	uint8_t continuous;

	if (x > 13 || y > 7) return 255;

	if (y % 2) {
		continuous = (y * N_COLS) + (N_COLS - (x + 1));
	} else {
		continuous = y * N_COLS + x;
	}
	return continuous;
}
#ifdef UNITTEST
void test_coord_to_snakish(void) {
    CU_ASSERT_EQUAL(coord_to_snakish(10, 3), 45);
    CU_ASSERT_EQUAL(coord_to_snakish(0, 0), 0);
    CU_ASSERT_EQUAL(coord_to_snakish(0, 1), 27);
    CU_ASSERT_EQUAL(coord_to_snakish(13, 7), 98);
    CU_ASSERT_EQUAL(coord_to_snakish(0, 7), 111);
}
#endif

void snakish_to_coord(uint8_t snake, uint8_t* x, uint8_t* y) {
	if (snake > N_COLS * N_ROWS) {
		*x = 255;
		*y = 255;
	} else {
		*y = snake / N_COLS;
		if (*y % 2) {
			*x = (N_COLS - 1) - (snake - (*y * N_COLS));
		} else {
			*x = snake - (*y * N_COLS);
		}
	}
}

float distance_coord(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
	//float x_sqr, y_sqr;
	uint16_t x_sqr, y_sqr;

	//x_sqr =  (float)x1 - (float)x2;
	//y_sqr =  (float)y1 - (float)y2;

	if (x1 > x2) x_sqr = x1 - x2; else x_sqr = x2 - x1;
	if (y1 > y2) y_sqr = y1 - y2; else y_sqr = y2 - y1;

    return sqrt((float)(x_sqr * x_sqr) + (float)(y_sqr * y_sqr));
}
#ifdef UNITTEST
void test_distance_coord(void) {
    CU_ASSERT_DOUBLE_EQUAL(distance_coord(0, 0, 1, 1), sqrt(2), DOUBLE_GRANULARITY);
    CU_ASSERT_DOUBLE_EQUAL(distance_coord(3, 4, 7, 8), sqrt(32), DOUBLE_GRANULARITY);
    CU_ASSERT_DOUBLE_EQUAL(distance_coord(7, 8, 3, 4), sqrt(32), DOUBLE_GRANULARITY);
    CU_ASSERT_DOUBLE_EQUAL(distance_coord(0, 1, 8, 1), sqrt(64), DOUBLE_GRANULARITY);
    CU_ASSERT_DOUBLE_EQUAL(distance_coord(0, 0, 14, 8), MAX_DISTANCE, DOUBLE_GRANULARITY);
}
#endif


float distance_snake(uint8_t pos1, uint8_t pos2) {
	uint8_t x1,x2,y1,y2;
	if (pos1 == pos2) {
		return 0.0;
	}
	snakish_to_coord(pos1, &x1, &y1);
	snakish_to_coord(pos2, &x2, &y2);

	return distance_coord(x1,y1,x2,y2);
}
#ifdef UNITTEST
void test_distance_snake(void) {
    CU_ASSERT_DOUBLE_EQUAL(distance_snake(0,   7),      7, DOUBLE_GRANULARITY);
    CU_ASSERT_DOUBLE_EQUAL(distance_snake(3,   4),      1, DOUBLE_GRANULARITY);
    CU_ASSERT_DOUBLE_EQUAL(distance_snake(3,  24),      1, DOUBLE_GRANULARITY);
    CU_ASSERT_DOUBLE_EQUAL(distance_snake(1, 110),      7, DOUBLE_GRANULARITY);
    CU_ASSERT_DOUBLE_EQUAL(distance_snake(0,   0),      0, DOUBLE_GRANULARITY);
}
#endif

#ifdef UNITTEST
char SUITE_NAME[] = "trans.c - position translation";

T_FUNC_DESCRIPTOR ALL_TEST_FUNCS[] = {
    { .fnc = &test_coord_to_snakish, .label = "test of coord_to_snakish()" },
    { .fnc = &test_distance_coord,   .label = "test of distance_coord()" },
    { .fnc = &test_distance_snake,   .label = "test of distance_snake()" },
};
#endif