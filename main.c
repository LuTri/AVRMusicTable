#include <avr/io.h>
#include <util/delay.h>
#include "ws2812/light_ws2812.h"
#include "hostcom/host.h"
#include <string.h>

#define UART_WHOLE_COLOR 'f'
#define N_LEDS 120

struct cRGB leds[N_LEDS];

void blocking_whole_color() {
	uint8_t idx,r,g,b;
	uint8_t start,end;
	
	r = uart_getc();
	g = uart_getc();
	b = uart_getc();	

	for (idx = 0; idx < N_LEDS; idx++) {
		leds[idx].r = r;
		leds[idx].g = g;
		leds[idx].b = b;
	}

	uart_putc('f');
	ws2812_setleds(leds,N_LEDS);
}

int main(void) {
	uint8_t value;
	uart_init();
	memset(leds,0,120*3);

	while(1) {
		if (1<<RXC0) {
			value = uart_getc();
			if (value == UART_WHOLE_COLOR) {
				blocking_whole_color();
			}
		}
	}
}
