#include <avr/io.h>
#include "modes.h"
#include "color.h"
#include <util/delay.h>

extern cRGB leds[N_LEDS];

uint8_t mode = CMD_MOOD;

float last_rand_h = 0;
float last_rand_v = 1.0;

void slave(void) {
	if (uart_available()) {
		read_uart((uint8_t*)&leds);
		ws2812_setleds();
		uart_puts("ack\n");
	}
}

void mood(void) {
	HSV hsv;
	RGB rgb;

	uint16_t idx;
	uint16_t tmp;

	last_rand_h += 0.02;
	if (last_rand_h > 360) {
		last_rand_h = 0;
	}

	hsv.h = last_rand_h;
	hsv.V = 0.13;
	hsv.S = 1.0;

	HsvToRgb(&hsv, &rgb);

//	tmp = rgb.r + rgb.g + rgb.b;

//	rgb.r = (255.0 / (float)tmp) * rgb.r;
//	rgb.g = (255.0 / (float)tmp) * rgb.g;
//	rgb.b = (255.0 / (float)tmp) * rgb.b;

	for (idx = 0; idx < N_LEDS; idx++) {
		leds[idx].r = rgb.r;
		leds[idx].g = rgb.g;
		leds[idx].b = rgb.b;
	}

	ws2812_setleds();
}

void sound(void) {
}
