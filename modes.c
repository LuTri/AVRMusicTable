#include <avr/io.h>
#include <util/delay.h>
#include "modes.h"
#include "color.h"
#include "ws2812.h"
#include "moods.h"

extern cRGB leds[N_PACKS];

uint8_t mode = CMD_WHITE;

void slave(void) {
	if (uart_available()) {
		read_uart((uint8_t*)&leds);
		ws2812_setleds();
		uart_puts("ack\n");
	}
}

void mood(void) {
	static uint8_t func_idx = 0;

	if (uart_available()) {
		func_idx = uart_getc();
	}

	fill_mood(0,1,1,func_idx);
	ws2812_setleds();
}

void sound(void) {
}

void white(void) {
	uint8_t idx;
	for (idx = 0; idx < N_PACKS; idx++) {
		leds[idx].r = 2;
		leds[idx].g = 2;
		leds[idx].b = 2;
	}
	ws2812_setleds();
}
