#include <avr/io.h>
#include "trans.h"
#include "config.h"

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
