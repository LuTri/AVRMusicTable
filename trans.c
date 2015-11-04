#include <avr/io.h>
#include "trans.h"
#include "globals.h"

#include <math.h>

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
	uint8_t x_sqr, y_sqr;
	x_sqr = x1 * x2;
	y_sqr = y1 * y2;

	return sqrt(y_sqr + x_sqr);
}

float distance_snake(uint8_t pos1, uint16_t pos2) {
	uint8_t x1,x2,y1,y2;
	if (pos1 == pos2) {
		return 0.0;
	}
	snakish_to_coord(pos1, &x1, &y1);
	snakish_to_coord(pos2, &x2, &y2);

	return distance_coord(x1,y1,x2,y2);
}
