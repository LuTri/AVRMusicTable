#include <avr/io.h>
#include <util/delay.h>
#include "modes.h"
#include "color.h"
#include "ws2812.h"
#include "moods.h"

extern cRGB leds[N_PACKS];

uint8_t mode = CMD_WHITE;

void slave(void) {
	uint16_t bench;
	char buf[2];

	if (uart_available()) {
		read_uart((uint8_t*)&leds);
		bench = ws2812_setleds();
		buf[0] = bench >> 8;
		buf[1] = bench;
		uart_putc(buf[0]);
		uart_putc(buf[1]);
	}
}

void mood(void) {
	static uint8_t func_idx = 0;

	if (uart_available()) {
		func_idx = uart_getc();
	}

	if ( fill_mood(0,100.0,1.0,func_idx) ) {
		ws2812_setleds();
	}
}

void sound(void) {
}

void white(void) {
	uint8_t idx;
	for (idx = 0; idx < N_PACKS; idx++) {
		leds[idx].r = leds[idx].g = leds[idx].b = 0;
	}
	ws2812_setleds();
}
