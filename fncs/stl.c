#include "functionalities.h"
#include "../display/color.h"
#include "../display/trans.h"

#define THRESHHOLD_STEP (0xFFFF / N_ROWS)
#define CMD_OFFSET 8

uint16_t dim_current = 0;
uint16_t dim_steps = 0;
float dimming = 0;

float target_hue = 0;
float hue_full = 0;
float intensity = 0;

RGB full_color;

static const __flash float hues[8] = {
    .0,
    20.0,
    62.0,
    115.0,
    120.0,
    120.0,
    120.0,
    120.0
};

void bar_row(uint16_t value, uint8_t column, uint8_t row) {
    uint16_t t_upper = THRESHHOLD_STEP * (N_ROWS - row);
    uint16_t t_lower = t_upper - THRESHHOLD_STEP;
    uint8_t snake_pos = coord_to_snakish(column, row);

    if (value > t_lower) {
        RGB color;
        //float hue = (
        //    (float)(value - t_lower) /
        //    (float)THRESHHOLD_STEP) * (target_hue - hue_full);

        fast_hsi(hues[row], ((float)value / (float)0xffff), &color);
        leds[snake_pos].r = color.r;
        leds[snake_pos].g = color.g;
        leds[snake_pos].b = color.b;
    } else {
        leds[snake_pos].r = 0;
        leds[snake_pos].g = 0;
        leds[snake_pos].b = 0;
    }
}

void stl_loop(void) {
    uint16_t values[N_COLS];

    dimming = (float)dim_current / (float)dim_steps;

    for (uint8_t idx = 0; idx < N_COLS; idx++) {
        values[idx] = dualbyte(((uint8_t*)loop_data)[(idx << 1) + CMD_OFFSET],
                               ((uint8_t*)loop_data)[(idx << 1) + CMD_OFFSET + 1]) * dimming;
    }

    for (uint8_t col = 0; col < N_COLS; col++) {
        for (uint8_t row = 0; row < N_ROWS; row++) {
            bar_row(values[col], col, row);
        }
    }

    ws2812_setleds();
    if (dim_current > 0) dim_current--;
}

void stl(COMMAND_BUFFER* command) {
    loop_fnc = 0;
    loop_data = command->data;
    hue_full = real_360_2byte(((uint8_t*)loop_data)[0],
                              ((uint8_t*)loop_data)[1]);
    target_hue = real_360_2byte(((uint8_t*)loop_data)[2],
                                ((uint8_t*)loop_data)[3]);
    intensity = per_one_2byte(((uint8_t*)loop_data)[4],
                              ((uint8_t*)loop_data)[5]);
    dim_current = dualbyte(((uint8_t*)loop_data)[6],
                           ((uint8_t*)loop_data)[7]);
    dim_steps = dim_current;

    fast_hsi(hue_full, intensity, &full_color);
    stl_loop();
    //loop_fnc = &stl_loop;
}
