#include "functionalities.h"

#include "../display/color.h"
#include "../display/trans.h"
#include "../AVRClock/benchmark.h"
#include "OdroidUart/avr-uart/uart.h"
#include "OdroidUart/pyconversion.h"
#include <math.h>

extern cRGB leds[N_PACKS];

uint8_t cycle_idx = 0;
uint8_t EXAMPL_DEBUG = 0;

uint8_t bmark_counter = 0;

typedef struct {
	float hue;
	float value;
	float distance;
	float angle;
} HVd;

void hue_pulse(HVd* hvd, RGB* vals) {
	float t_h, t_v;

	float _tmp;

	_tmp = (hvd->distance / MAX_DISTANCE);

	t_h = (hvd->hue + cycle_idx * 3);
	t_h = t_h - (t_h * _tmp);

	t_v = hvd->value;
	t_v = t_v - (t_v * _tmp);

	fast_hsi(t_h, t_v, vals);
}

void hue_rotate(HVd* hvd, RGB* vals) {
	float t_h, t_v = hvd->value;

    t_h = ((cycle_idx * 3) - (hvd->angle / M_PI * 180)) - 180;
	t_v = t_v - t_v * (hvd->distance / MAX_DISTANCE);
	fast_hsi(t_h, t_v, vals);
}

void mood3(HVd* hvd, RGB* vals) {
}

void mood4(HVd* hvd, RGB* vals) {
}

void (*funcs[])(HVd*, RGB*) = {hue_pulse, hue_rotate, mood3, mood4};

uint8_t pos = 0;

uint8_t fill_mood(uint8_t center_y, uint8_t center_x, float starting_h, float starting_v,
	uint8_t func_idx) {
	uint8_t idx;
	uint8_t _y, _x;
	void (*func)(HVd*, RGB*) = funcs[func_idx];

	RGB rgb;
	HVd hvd;

	hvd.hue = starting_h;
	hvd.value = starting_v;

	for (idx = 0; idx < N_PACKS; idx++) {
		snakish_to_coord(idx, &_x, &_y);
		hvd.distance = distance_coord(_x, _y, center_x, center_y);
		hvd.angle = atan2f((float)_y - center_y, (float)_x - center_x);

		(*func)(&hvd, &rgb);

		leds[idx].r = (rgb.r >> 4);
		leds[idx].g = (rgb.g >> 4);
		leds[idx].b = (rgb.b >> 4);
	}
	cycle_idx++;

	if (cycle_idx == 120) {
		cycle_idx = 0;
	}

	return 1;
}

void mood_loop(void) {
	static uint16_t fnc_counter = 0;

	uint8_t _benchmarking = 0;

	uint8_t pos_y, pos_x, mood_idx;
	uint16_t threshold;

	float hue, intensity;

	pos_x = ((uint8_t*)loop_data)[0];
	pos_y = ((uint8_t*)loop_data)[1];

	hue = real_360_2byte(((uint8_t*)loop_data)[2],  ((uint8_t*)loop_data)[3]);
	intensity = per_one_2byte(((uint8_t*)loop_data)[4],  ((uint8_t*)loop_data)[5]);
	threshold = dualbyte(((uint8_t*)loop_data)[6], ((uint8_t*)loop_data)[7]);
	mood_idx = ((uint8_t*)loop_data)[8];

	if (fnc_counter++ >= threshold) {
	    if (DO_BENCHMARK && bmark_counter < UART_NUM_BENCHMARKS) {
	        _benchmarking = 1;
	        benchmark_start();
	        bmark_counter++;
	    }
		fnc_counter = 0;
		fill_mood(pos_y, pos_x, hue, intensity, mood_idx);
        if (DO_BENCHMARK && _benchmarking) {
            benchmark_stop(0);
            if (bmark_counter == 1) uart0_puts(MSG_BENCHMARK_START);
            publish_benchmark(&uart0_puts, &uart0_putc, MSG_BENCHMARK_DATA);
            if (bmark_counter == UART_NUM_BENCHMARKS) uart0_puts(MSG_BENCHMARK_STOP);
        }
        ws2812_setleds();
	}
}

void launch_mood(COMMAND_BUFFER* command) {
	loop_data = command->data;
    bmark_counter = 0;
	loop_fnc = &mood_loop;
}