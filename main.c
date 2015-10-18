#include <avr/io.h>

#include "config.h"
#include "ws2812/light_ws2812.h"
#include "hostcom/host.h"


extern cRGB leds[N_LEDS];

int main(void) {
	uint8_t val = STAT_LED;
	DDRB = STAT_LED;
	
	uart_init();

	while(1) {
		if (uart_available()) {
			read_uart((uint8_t*)&leds);
			ws2812_setleds();
			uart_puts("ack\n");
		}
		
	}
}
