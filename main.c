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
	TCCR1 |= (1<<CS10);
	sei();
}

void stop_timer(void) {
	tim_counter = TCNT1;
	TCCR1 &= ~(1<<CS10);
}

int main(void) {
	uart_init();

	while(1) {
		start_timer();
		read_uart((cRGB*)&leds);
		stop_timer();
//		if (uart_available()) {
//			value = uart_getc();
//			if (value == UART_WHOLE_COLOR) {
//				blocking_whole_color();
//			}
//		}
	}
}
