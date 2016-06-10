#include <avr/io.h>
#include <util/delay.h>
#include "modes.h"
#include "color.h"
#include "ws2812.h"
#include "moods.h"

extern cRGB leds[N_PACKS];

uint8_t mode = CMD_SLAVEMODE;

void slave(void) {
	char buf[2];

	if (uart_available()) {
		read_uart((uint8_t*)&leds);
		ws2812_setleds();
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
		leds[idx].r = leds[idx].g = leds[idx].b = 255;
	}
	ws2812_setleds();
}

void off(void) {
	uint8_t idx;
	for (idx = 0; idx < N_PACKS; idx++) {
		leds[idx].r = leds[idx].g = leds[idx].b = 0;
	}
	ws2812_setleds();
}

void byte(uint16_t value) {
	uint8_t idx;
	for (idx = 0; idx < 16; idx++) {
		leds[idx].r = leds[idx].g = leds[idx].b = 255 * (1 & (value >> idx));
	}
	ws2812_setleds();
}

void byte_offset(uint8_t value, uint8_t offset) {
	uint8_t idx;
	if (offset + 8 < N_PACKS) {
		for (idx = 0; idx < 8; idx++) {
			leds[idx + offset].r = leds[idx + offset].g = leds[idx + offset].b = 255 * (1 & (value >> idx));
		}
		ws2812_setleds();
	}
}

void boot(void) {
	uint8_t idx;
	for (idx = 0; idx < N_PACKS; idx++) {
		leds[idx].r = 255 * (1 & (idx));
		leds[idx].g = 255 * (1 & (idx >> 1));
		leds[idx].b = 255 * (1 & (idx >> 2));
	}
	ws2812_setleds();
}
