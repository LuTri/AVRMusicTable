#include <avr/io.h>
#include "modes.h"
#include "color.h"
#include "config.h"
#include <util/delay.h>
#include "moods.h"

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
	RGB rgb;

	uint16_t idx;

	last_rand_h += 1;
	if (last_rand_h > 900) {
		last_rand_h = 0;
	}

	fast_hsi(last_rand_h, 0.2, &rgb);

	for (idx = 0; idx < N_LEDS; idx++) {
		leds[idx].r = rgb.r;
		leds[idx].g = rgb.g;
		leds[idx].b = rgb.b;
	}

	ws2812_setleds();
}

void sound(void) {
}
