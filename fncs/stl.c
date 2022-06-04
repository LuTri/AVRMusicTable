#include "functionalities.h"
#include "../display/color.h"
#include "../display/trans.h"
#include "../state.h"

#define THRESHHOLD_STEP (0xFFFF / N_ROWS)

uint16_t fnc_counter = 0;
uint16_t dim_current = 0;
uint8_t new_vals = 0;

float dimming = 0;
float intensity = 0;

void bar_row(uint16_t value, uint8_t column, uint8_t row, float* hues) {
    uint8_t snake_pos = coord_to_snakish(column, row);

    if (value > THRESHHOLD_STEP * (N_ROWS - (row + 1))) {
        RGB color;
        fast_hsi(hues[row], ((float)value / (float)0xffff) * intensity, &color);
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
    float* hues = GET_STATE(stl_hues);

    uint16_t tmp_val;

    if (fnc_counter != GET_STATE(stl_fnc_counts)) {
        fnc_counter++;
        return;
    }
    fnc_counter = 0;

    if (dim_current == 0) return;

    dimming = (float)dim_current / (float)GET_STATE(stl_dim_counts);
    dim_current--;

    for (uint8_t idx = 0; idx < N_COLS; idx++) {
        if (new_vals) {
            previous[idx] = current[idx];
            current[idx] = dualbyte(
                ((uint8_t*)loop_data)[(idx << 1)],
                ((uint8_t*)loop_data)[(idx << 1) + 1]
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
            bar_row(values[col], col, row, hues);
        }
    }

    new_vals = 0;
    ws2812_setleds();
}

void stl(COMMAND_BUFFER* command) {
    loop_fnc = 0;
    loop_data = command->data;
    uint8_t* int_data = (uint8_t*)&GET_STATE(stl_intensity);

    intensity = per_one_2byte(int_data[0],int_data[1]);
    dim_current = GET_STATE(stl_dim_counts);
    fnc_counter = GET_STATE(stl_fnc_counts);

    new_vals = 1;

    loop_fnc = &stl_loop;
}
