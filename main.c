#include <avr/io.h>
#include <util/delay.h>
#include "ws2812/light_ws2812.h"
#include "hostcom/host.h"
#include <string.h>

#define UART_WHOLE_COLOR 'f'

extern cRGB leds[N_LEDS];

volatile uint16_t ovf_counter = 0;
volatile uint16_t tim_counter = 0;

/*void blocking_whole_color() {
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
	ws2812_setleds();
}*/

ISR(TIMER1_OVF_vect) {
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
			start_timer();
			read_uart((cRGB*)&leds);
			stop_timer();
			for (idx = 0; idx < 100; idx++) {
				buf[idx] = '\0';
			}
			utoa(ovf_counter, buf, 10);
			for (idx = 0; idx < 100; idx++) {
				if (buf[idx] == '\0') {
					buf[idx] = ' ';
					buf[idx + 1] = 'O';
					buf[idx + 2] = '\n';
					break;
				}
			}
			uart_puts(buf);
			for (idx = 0; idx < 100; idx++) {
				buf[idx] = '\0';
			}
			utoa(tim_counter, buf, 10);
			for (idx = 0; idx < 100; idx++) {
				if (buf[idx] == '\0') {
					buf[idx] = ' ';
					buf[idx + 1] = 'T';
					buf[idx + 2] = '\n';
					break;
				}
			}
			uart_puts(buf);
			ovf_counter = 0;
			tim_counter = 0;
//		if (uart_available()) {
//			value = uart_getc();
//			if (value == UART_WHOLE_COLOR) {
//				blocking_whole_color();
//			}
//		}
		}
	}
}
