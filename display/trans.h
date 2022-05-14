#ifndef _TRANS_H
#define _TRANS_H

#ifdef UNITTEST
#include "unittest.h"
#endif

uint8_t coord_to_snakish(uint8_t x, uint8_t y);
void snakish_to_coord(uint8_t snake, uint8_t* x, uint8_t* y);

float distance_coord(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
float distance_snake(uint8_t pos1, uint8_t pos2);

#define MAX_DISTANCE 16.124515f

#endif
