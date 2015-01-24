#include <avr/io.h>
#include <util/delay.h>
#include "ws2812/light_ws2812.h"
#include "hostcom/host.h"
#include <string.h>

#define UART_WHOLE_COLOR 'f'

extern cRGB leds[N_LEDS];

volatile uint16_t ovf_counter = 0;
volatile uint16_t tim_counter = 0;

/*ISR(TIMER1_OVF_vect) {
	ovf_counter++;
}

void start_timer(void) {
	TIMSK1 |= (1<<TOIE1);
	TCCR1B |= (1<<CS10);
	sei();
}

void stop_timer(void) {
	tim_counter = TCNT1;
	TCCR1B &= ~(1<<CS10);
	TCNT1 = 0;
}
*/

int main(void) {
	uint8_t idx;
	char buf[100];
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
