#include "functionalities.h"
#include "../display/color.h"
#include "../display/trans.h"

#define THRESHHOLD_STEP (0xFFFF / N_ROWS)
#define STL_STEPS 200

uint8_t stl_status = 0;

float hue_full = 0;
RGB full_color;

void bar_row(uint16_t value, uint8_t column, uint8_t row) {
    uint8_t snake_pos = coord_to_snakish(column, row);
    uint16_t fraction = value - (THRESHHOLD_STEP * (N_ROWS - (row + 1)));

    float hue = ((float)fraction / (float)THRESHHOLD_STEP) * 360.0;

    if (fraction >= THRESHHOLD_STEP) {
        leds[snake_pos].r = full_color.r;
        leds[snake_pos].g = full_color.g;
        leds[snake_pos].b = full_color.b;
    } else if (fraction > 0) {
        fast_hsi(hue_full + hue, 0.5, (RGB*)(&(leds[snake_pos])));
    } else {
        leds[snake_pos].r = 0;
        leds[snake_pos].g = 0;
        leds[snake_pos].b = 0;
    }
}

void stl_loop(void) {
    uint16_t values[N_COLS];

    for (uint8_t idx = 0; idx < N_COLS; idx++) {
        values[idx] = dualbyte(((uint8_t*)loop_data)[((idx + 1) << 1)],
                               ((uint8_t*)loop_data)[((idx + 1) << 1) + 1]);
    }

    for (uint8_t col = 0; col < N_COLS; col++) {
        for (uint8_t row = 0; row < N_ROWS; row++) {
            bar_row(values[col], col, row);
        }
    }

    ws2812_setleds();
    if (stl_status > 0) stl_status--;
}

void stl(COMMAND_BUFFER* command) {
    hue_full = real_360_2byte(((uint8_t*)loop_data)[0],
                              ((uint8_t*)loop_data)[1]);

    fast_hsi(hue_full, .5, &full_color);

    stl_status = STL_STEPS;
    loop_data = command->data;
    loop_fnc = &stl_loop;
}