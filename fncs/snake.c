#include "functionalities.h"
#include "../OdroidUart/pyconversion.h"

uint8_t _projection_distance(uint8_t led_idx, uint8_t snake_idx, uint8_t* is_front) {
    uint8_t tmp;
    if (led_idx > snake_idx) {
        *is_front = 1;
        tmp = led_idx - snake_idx;
    } else {
        tmp = snake_idx - led_idx;
        *is_front = 0;
    }

    if (tmp > N_PACKS / 2) {
        *is_front ^= 1;
        return N_PACKS - tmp;
    }
    return tmp;
}

void _snake(uint8_t idx,
            uint8_t r, uint8_t g, uint8_t b,
            uint8_t t_r_f, uint8_t t_g_f, uint8_t t_b_f,
            uint8_t t_r_b, uint8_t t_g_b, uint8_t t_b_b,
            uint8_t len_front, uint8_t len_back) {
    uint8_t loop_idx, distance, is_front;

    float
    step_r_f = (float)(r - t_r_f) / (float)(len_front),
    step_g_f = (float)(g - t_g_f) / (float)(len_front),
    step_b_f = (float)(b - t_b_f) / (float)(len_front),
    step_r_b = (float)(r - t_r_b) / (float)(len_back),
    step_g_b = (float)(g - t_g_b) / (float)(len_back),
    step_b_b = (float)(b - t_b_b) / (float)(len_back);

    for (loop_idx = 0; loop_idx < N_PACKS; loop_idx++) {
        uint8_t* cmp;
        float *step_r, *step_g, *step_b;
        distance = _projection_distance(loop_idx, idx, &is_front);

        if (is_front) {
            cmp = &len_front;
            step_r = &step_r_f;
            step_g = &step_g_f;
            step_b = &step_b_f;
        } else {
            cmp = &len_back;
            step_r = &step_r_b;
            step_g = &step_g_b;
            step_b = &step_b_b;
        }

        if (distance <= *cmp) {
            set_led(loop_idx,
                    (uint8_t)(r - (*step_r) * distance),
                    (uint8_t)(g - (*step_g) * distance),
                    (uint8_t)(b - (*step_b) * distance));
        } else {
            set_led(loop_idx, 0, 0, 0);
        }
    }
    ws2812_setleds();
}

void snake_loop(void) {
    static uint16_t fnc_counter = 0;
    static uint8_t snake_pos = 0;
    uint16_t threshold;
    uint8_t r, g, b, t_r_f, t_g_f, t_b_f, t_r_b, t_g_b, t_b_b, len_front, len_back;

    threshold = dualbyte(((uint8_t*)loop_data)[0], ((uint8_t*)loop_data)[1]);
    r = ((uint8_t*)loop_data)[2];
    g = ((uint8_t*)loop_data)[3];
    b = ((uint8_t*)loop_data)[4];
    t_r_f = ((uint8_t*)loop_data)[5];
    t_g_f = ((uint8_t*)loop_data)[6];
    t_b_f = ((uint8_t*)loop_data)[7];
    t_r_b = ((uint8_t*)loop_data)[8];
    t_g_b = ((uint8_t*)loop_data)[9];
    t_b_b = ((uint8_t*)loop_data)[10];
    len_front = ((uint8_t*)loop_data)[11];
    len_back = ((uint8_t*)loop_data)[12];

    if (fnc_counter++ >= threshold) {
        _snake(snake_pos++, r, g, b, t_r_f, t_g_f, t_b_f, t_r_b, t_g_b, t_b_b, len_front, len_back);
        if (snake_pos >= N_PACKS) {
            snake_pos = 0;
        }
        fnc_counter = 0;
    }
}

void snake(COMMAND_BUFFER* command) {
    loop_data = command->data;
    loop_fnc = &snake_loop;
}