#include "ws2812.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

cRGB leds[N_PACKS];

void ws2812_sendarray_mask(void);
 
uint16_t ws2812_setleds(void) {
	TCNT1 = 0;
	TCCR1B |= (1<<CS10); // Prescaler 64
	ws2812_DDRREG |= ws2812_pin; // Enable DDR
	ws2812_sendarray_mask();
	_delay_us(50);
	TCCR1B &= ~(1<<CS10); // off
	return TCNT1;
}

void inline ws2812_sendarray_mask(void) {
	uint8_t* data = (uint8_t*)(leds);
	uint16_t datlen = N_LEDS;
	uint8_t maskhi = ws2812_pin;
	uint8_t curbyte,ctr,masklo;
	uint8_t sreg_prev;
	
	masklo	=~maskhi&ws2812_PORTREG;
	maskhi |=		ws2812_PORTREG;
	sreg_prev=SREG;
	cli();	

	while (datlen--) {
	curbyte=*data++;
	
		asm volatile(
			"		ldi	%0,8	\n\t"
			"loop%=:			\n\t"
			"		out	%2,%3	\n\t"	//	'1' [01] '0' [01] - re
			"	nop				\n\t"
			"	rjmp .+0		\n\t"
			"		sbrs	%1,7\n\t"	//	'1' [03] '0' [02]
			"		out	%2,%4	\n\t"	//	'1' [--] '0' [03] - fe-low
			"		lsl	%1		\n\t"	//	'1' [04] '0' [04]
			"	nop				\n\t"
			"	rjmp .+0		\n\t"
			"	rjmp .+0		\n\t"
			"	rjmp .+0		\n\t"
			"		out	%2,%4	\n\t"	//	'1' [+1] '0' [+1] - fe-high
			"	rjmp .+0		\n\t"
			"		dec	%0		\n\t"	//	'1' [+2] '0' [+2]
			"		brne loop%=	\n\t"	//	'1' [+3] '0' [+4]
			:	"=&d" (ctr)
			:	"r" (curbyte),
				"I" (_SFR_IO_ADDR(ws2812_PORTREG)),
				"r" (maskhi),
				"r" (masklo)
		);
	}
	
	SREG=sreg_prev;
}
