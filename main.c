#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "globals.h"
#include "modes.h"

#include "AVRClock/customtimer.h"

extern cRGB leds[N_LEDS];

int main(void) {
	uint8_t val = 0b00100000;
	DDRB = 0b00100000;
	
	uart_init();

	while(1) {
		if (uart_available()) {
			val ^= 0b00100000;
			PORTB = val;
			read_uart((uint8_t*)&leds);
			uart_puts("ack\n");
			ws2812_setleds();
		}
		
	}
}
