#ifndef _FONT_H
#define _FONT_H

#include <avr/io.h>

#define FONT_WIDTH 5
#define IDX_SPACE

uint8_t font_pixel(uint8_t _char, uint8_t x, uint8_t y);
uint8_t get_font(uint8_t idx, uint8_t column);

#endif