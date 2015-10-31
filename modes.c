#include <avr/io.h>
#include "modes.h"
#include "color.h"
#include "config.h"
#include <util/delay.h>
#include "moods.h"

extern cRGB leds[N_LEDS];

uint8_t mode = CMD_MOOD;

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
