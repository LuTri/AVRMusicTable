#include <avr/io.h>
#include <util/delay.h>
#include "modes.h"
#include "color.h"
#include "ws2812.h"
#include "moods.h"

#define FRAMEERROR (1 << FE0)
#define OVERFLOWERROR (1 << DOR0)
#define PARITYERROR (1 << UPE0)

extern cRGB leds[N_PACKS];

uint8_t mode = CMD_WHITE;

void light_n_leds(uint16_t n) {
    uint16_t idx;
    off();
    for (idx = 0; idx < n; idx++) {
        leds[idx].r = 0;
        leds[idx].g = 128;
        leds[idx].b = 255;
    }

    ws2812_setleds();
}

void light_uart_status(void) {
    uint16_t idx = uart_status_idx;
    uint8_t status;
    uint8_t doit = 0;
    while (--idx) {
        status = UART_STATI[idx];
        if (status & FRAMEERROR) {
            leds[idx].r = 255;
            doit = 1;
        }

        if (status & OVERFLOWERROR) {
            leds[idx].g = 255;
            doit = 1;
        }

        if (status & PARITYERROR) {
            leds[idx].b = 255;
            doit = 1;
        }
    }
    if (doit) {
        ws2812_setleds();
    }
}

uint8_t _projection_distance(uint8_t led_idx, uint8_t snake_idx) {
    uint8_t tmp;
    if (led_idx > snake_idx)
        tmp = led_idx - snake_idx;
    else
        tmp = snake_idx - led_idx;
    
    if (tmp > N_PACKS / 2)
        return N_PACKS - tmp;
    return tmp;
}

void snake(uint8_t idx, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t loop_idx, distance;

    for (loop_idx = 0; loop_idx < N_PACKS; loop_idx++) {
        distance = _projection_distance(loop_idx, idx);
        if (distance < MAX_SNAKE_LENGTH) {
            leds[loop_idx].r = r >> (distance - 2);
            leds[loop_idx].g = g >> (distance * 2);
            leds[loop_idx].b = b >> distance;
        } else {
            leds[loop_idx].r = 0;
            leds[loop_idx].g = 0;
            leds[loop_idx].b = 0;
        }
    }
    ws2812_setleds();
}

void slave(void) {
    uint8_t idx = 0;
    uint8_t status = 255;
    uint16_t size;
    size = uart_prot_read((uint8_t*)&leds, N_LEDS, &status);
    if (size > 0) {
        ws2812_setleds();
    }
    light_uart_status();
}

void rgb(uint8_t r, uint8_t g, uint8_t b) {
	uint8_t idx;
	for (idx = 0; idx < N_PACKS; idx++) {
		leds[idx].r = r;
        leds[idx].g = g;
        leds[idx].b = b;
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
