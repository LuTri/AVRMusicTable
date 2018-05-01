#include <avr/io.h>
#include <util/delay.h>
#include "modes.h"
#include "color.h"
#include "ws2812.h"
#include "moods.h"

extern cRGB leds[N_PACKS];

uint8_t mode = CMD_WHITE;

void slave(void) {
    uint16_t size;
    size = uart_prot_read((uint8_t*)&leds, N_LEDS);
    ws2812_setleds();
}

void rgb(uint8_t r, uint8_t g, uint8_t b) {
	uint8_t idx;
	for (idx = 0; idx < N_PACKS; idx++) {
		leds[idx].r = r;
        leds[idx].g = g;
        leds[idx].b = b;;
	}
	ws2812_setleds();
}

void sound(void) {
}

void white(void) {
    rgb(255,255,255);
}

void red(void) {
    rgb(255,0,0);
}

void blue(void) {
    rgb(0,0,255);
}

void green(void) {
    rgb(0,255,0);
}

void off(void) {
    rgb(0,0,0);
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
