#include <avr/io.h>

#include "config.h"
#include "ws2812/light_ws2812.h"
#include "hostcom/host.h"


extern cRGB leds[N_LEDS];

int main(void) {
	uint8_t val = 0b00100000;
	DDRB = 0b00100000;
	
	uart_init();

	while(1) {
		if (UART_FLAGS & (1<<1)) {
			val ^= 0b00100000;
			PORTB = val;
			read_uart((uint8_t*)&leds);
			uart_puts("ack\n");
			ws2812_setleds();
		}
		
	}
}
