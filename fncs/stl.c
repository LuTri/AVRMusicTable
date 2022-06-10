#include "functionalities.h"
#include "../display/color.h"
#include "../display/trans.h"
#include "../state.h"
#include "../OdroidUart/pyconversion.h"

#define THRESHHOLD_STEP (0xFFFF / N_ROWS)

#define N_DIMS 20

static const __flash float fading[N_DIMS] = {
    1.0,
    0.9639,
    0.9261,
    0.8864,
    0.8448,
    0.8014,
    0.7562,
    0.7091,
    0.6602,
    0.6094,
    0.5567,
    0.5023,
    0.4459,
    0.3878,
    0.3277,
    0.2659,
    0.2022,
    0.1366,
    0.0692,
    0.0
};

uint8_t new_stl = 0;

void bar_row(uint16_t value, uint8_t column, uint8_t row, float intensity, float* hues) {
    RGB color;
    uint8_t snake_pos = coord_to_snakish(column, row);

    if (value > THRESHHOLD_STEP * (N_ROWS - (row + 1))) {
        fast_hsi(hues[row], ((float)value / (float)0xffff) * intensity, &color);
        set_led(snake_pos, color.r, color.g, color.b);
    } else {
        set_led(snake_pos, 0, 0, 0);
    }
}

void stl_loop(void) {
    static uint16_t fnc_counter = 0;
    static uint8_t fade_idx[N_COLS] = { 0 };
    static uint16_t current[N_COLS] = { 0 };

    uint16_t new_vals[N_COLS];
    uint8_t col, row;

    float* hues = GET_STATE(stl_hues);
    uint8_t darks = 0;
    float intensity = per_one_2byte(
        ((uint8_t*)&GET_STATE(stl_intensity))[0],
        ((uint8_t*)&GET_STATE(stl_intensity))[1]
    );

    if (fnc_counter < GET_STATE(stl_fnc_counts)) {
        fnc_counter++;
        return;
    }
    fnc_counter = 0;

    for (col = 0; col < N_COLS; col++) {
        if (new_stl) {
            new_vals[col] = dualbyte(
                ((uint8_t*)loop_data)[(col << 1)],
                ((uint8_t*)loop_data)[(col << 1) + 1]
            );

            if (new_vals[col] > current[col]) {
                current[col] = new_vals[col];
                fade_idx[col] = 0;
            }
        }

        current[col] *= fading[fade_idx[col]];
        if (fade_idx[col] < (N_DIMS - 1)) fade_idx[col] += 1;
        else darks++;

        for (row = 0; row < N_ROWS; row++) {
            bar_row(current[col], col, row, intensity, hues);
        }
    }

    new_stl = 0;
    ws2812_setleds();

    /* Don't loop when there's nothing to loop over!*/
    if (darks == N_COLS) {
        loop_fnc = 0;
    }
}

void stl(COMMAND_BUFFER* command) {
    loop_fnc = 0;
    loop_data = command->data;

    new_stl = 1;

    loop_fnc = &stl_loop;
}
