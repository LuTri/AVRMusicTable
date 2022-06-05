#include "functionalities.h"
#include "../display/trans.h"
#include "../text/font.h"
#include "../OdroidUart/pyconversion.h"

uint8_t translate_viewport_col(short offset, uint8_t col, short* roll_idx) {
    *roll_idx = (col + offset) / FONT_WIDTH;
    return (col + offset) % FONT_WIDTH;
}

uint16_t loop_counter = 0;
short viewport_offset = -N_COLS;

/* create a rapidly fading after image to the right */
void apply_ghosting(uint8_t col, uint8_t row, uint8_t idx, uint8_t s_r, uint8_t s_g, uint8_t s_b) {
    uint8_t neighbour_idx;
    if (col < N_COLS - 1) {
        neighbour_idx = coord_to_snakish(col + 1, row);
        set_led(neighbour_idx, s_r, s_g, s_b);
    }
}

/* Make chars dimmer near the edgers */
void apply_fading(uint8_t col, uint8_t row, uint8_t idx) {
    /* 1  1  1  1  1  1  1  1  1  1  1  1  1  1  ->
      .1.15 .2 .3 .6  1  1  1  1 .9 .8 .7 .4 .1 */
    static const __flash float factors[] = {.1, .15, .2, .3, .6, 1, 1};
    uint8_t _fac_idx = col;
    float factor;

    if (col >= N_COLS) return;

    if (_fac_idx >= (N_COLS / 2)) {
        _fac_idx = N_COLS - (_fac_idx + 1);
    }

    factor = factors[_fac_idx];

    set_led(idx,
            (uint8_t)(factor * leds[idx].r),
            (uint8_t)(factor * leds[idx].g),
            (uint8_t)(factor * leds[idx].b));
}

void _text_looper(void) {
    uint8_t row, col;
    uint16_t stepper = dualbyte(loop_data[0], loop_data[1]);
    uint8_t char_r = loop_data[2];
    uint8_t char_g = loop_data[3];
    uint8_t char_b = loop_data[4];

    uint8_t fading = loop_data[5];
    uint8_t ghosting = loop_data[6];

    uint8_t ghost_r = (uint8_t)((float)char_r * .1f);
    uint8_t ghost_g = (uint8_t)((float)char_g * .1f);
    uint8_t ghost_b = (uint8_t)((float)char_b * .1f);

    uint16_t dat_len = dualbyte(loop_data[7], loop_data[8]);
    uint8_t* data = &loop_data[9];

    short roll_idx = 0;

    if (!(loop_counter++ == stepper)) return;
    loop_counter = 0;

    for (uint8_t idx = 0; idx < N_PACKS; idx++) {
        set_led(idx, 0, 0, 0);
    }

    for (col = 0; col < N_COLS; col++) {
        uint8_t t_col = translate_viewport_col(viewport_offset, col, &roll_idx);

        for (row = 0; row < N_ROWS; row++) {
            if (roll_idx < (short)dat_len && roll_idx >= 0 && font_pixel(data[roll_idx], t_col, row)) {
                /* skip pre   &&   on if pixel   */
                uint8_t idx = coord_to_snakish(col, row);
                set_led(idx, char_r, char_g, char_b);

                if (fading) apply_fading(col, row, idx);
                if (ghosting) {
                    apply_ghosting(col, row, idx, ghost_r, ghost_g, ghost_b);
                    if (fading) apply_fading(col + 1, row, idx);
                }
            }
        }
    }

    ws2812_setleds();
    if (++viewport_offset >= ((short)dat_len * FONT_WIDTH)) viewport_offset = -N_COLS;
}

void text_writer(COMMAND_BUFFER* command) {
    loop_data = command->data;
    loop_counter = 0;
    viewport_offset = -N_COLS;
    loop_fnc = &_text_looper;
}