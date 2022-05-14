#include "font.h"

uint8_t is_pixel_for_char(uint8_t _char, uint8_t x, uint8_t y) {
    if (x >= 5 || y >= 8) return 0;

    return get_font(_char, x) & (1 << (7 - y));

    return 1;
}