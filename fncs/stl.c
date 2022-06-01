#include "functionalities.h"
#include "../display/color.h"
#include "../display/trans.h"

#define THRESHHOLD_STEP (0xFFFF / N_ROWS)
#define CMD_OFFSET 6

uint16_t fnc_counter = 0;
uint16_t dim_current = 0;
uint16_t dim_steps = 0;
uint16_t dim_delay = 0;
uint8_t new_vals = 0;

float dimming = 0;

float intensity = 0;

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
    static uint16_t values[N_COLS] = { 0 };
    static uint16_t previous[N_COLS];
    static uint16_t current[N_COLS];

    uint16_t tmp_val;

    if (fnc_counter != dim_delay) {
        fnc_counter++;
        return;
    }
    fnc_counter = 0;

    if (dim_current == 0) return;

    dimming = (float)dim_current / (float)dim_steps;
    dim_current--;

    for (uint8_t idx = 0; idx < N_COLS; idx++) {
        if (new_vals) {
            previous[idx] = current[idx];
            current[idx] = dualbyte(
                ((uint8_t*)loop_data)[(idx << 1) + CMD_OFFSET],
                ((uint8_t*)loop_data)[(idx << 1) + CMD_OFFSET + 1]
            );
        }

        if (current[idx] > previous[idx]) {
            tmp_val = current[idx] * dimming;
        } else {
            tmp_val = previous[idx] * dimming;
        }

        values[idx] = tmp_val;
    }

    for (uint8_t col = 0; col < N_COLS; col++) {
        for (uint8_t row = 0; row < N_ROWS; row++) {
            bar_row(values[col], col, row);
        }
    }

    new_vals = 0;
    ws2812_setleds();
}

void stl(COMMAND_BUFFER* command) {
    loop_fnc = 0;
    loop_data = command->data;
    intensity = per_one_2byte(((uint8_t*)loop_data)[0],
                              ((uint8_t*)loop_data)[1]);
    dim_current = dualbyte(((uint8_t*)loop_data)[2],
                           ((uint8_t*)loop_data)[3]);
    dim_delay = dualbyte(((uint8_t*)loop_data)[4],
                         ((uint8_t*)loop_data)[5]);
    dim_steps = dim_current;
    fnc_counter = dim_delay;

    new_vals = 1;

    loop_fnc = &stl_loop;
}
