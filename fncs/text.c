#include "functionalities.h"
#include "../ws2812.h"
#include "../trans.h"
#include "../text/font.h"

uint8_t translate_viewport_col(short offset, uint8_t col, short* roll_idx) {
    *roll_idx = (col + offset) / FONT_WIDTH;
    return (col + offset) % FONT_WIDTH;
}

uint16_t loop_counter = 0;
short viewport_offset = -N_COLS;

void _text_looper(void) {
    uint8_t row, col;
    uint16_t stepper = dualbyte(loop_data[0], loop_data[1]);
    uint16_t dat_len = dualbyte(loop_data[4], loop_data[5]);
    uint8_t* data = &loop_data[6];

    short roll_idx = 0;

    if (!(loop_counter++ == stepper)) return;
    loop_counter = 0;

    for (uint8_t idx = 0; idx < N_PACKS; idx++) {
        leds[idx].r = 0;
        leds[idx].g = 0;
        leds[idx].b = 0;

        snakish_to_coord(idx, &col, &row);

        col = translate_viewport_col(viewport_offset, col, &roll_idx);

        if (roll_idx < (short)dat_len && roll_idx >= 0 && is_pixel_for_char(data[roll_idx], col, row)) {
           /* skip pre   &&   on if pixel   */
            leds[idx].b = 10;
        }
    }

    ws2812_setleds();
    if (++viewport_offset >= ((short)dat_len * FONT_WIDTH)) viewport_offset = -N_COLS;
}

void text_writer(COMMAND_BUFFER* command) {
    for (uint8_t i = 0; i < 60; i++) {
        ((uint8_t*)loop_data)[i] = command->data[i];
    }

    loop_counter = 0;
    viewport_offset = -N_COLS;
    loop_fnc = &_text_looper;
}