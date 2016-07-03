#include "ws2812.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

cRGB leds[N_PACKS];

void ws2812_sendarray_mask(void);
 
void ws2812_setleds(void) {
	ws2812_DDRREG |= ws2812_pin; // Enable DDR
	ws2812_sendarray_mask();
	_delay_us(50);
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
			"		ldi	%0,8	\n\t"	// C: 1		0	| set ctr := 8
			"loop%=:			\n\t"	// C: 0		1	| LABEL loop
			"		out	%2,%3	\n\t"	// C: 1		1	| SET ws2812_portreg == MASKHI - '1' [01] '0' [01] - re
			"	nop				\n\t"	// C: 1		2	| NO OPERATION
			"	rjmp .+0		\n\t"	// C: 2		3	| JUMP 1 FORWARD
			"		sbrs	%1,7\n\t"	// C: 1		5	| SKIP NEXT INSTRUCTION IF curbyte [7] == 1	'1' [03] '0' [02]
			"		out	%2,%4	\n\t"	// C: 1		6	| SET ws2812_portreg == MASKLO - '1' [--] '0' [03] - fe-low
			"		lsl	%1		\n\t"	// C: 1		7	| LEFTSHIFT OF curbyte (e.g. 0b001 -> 0b010) '1' [04] '0' [04]
			"	nop				\n\t"	// C: 1		8	| NO OPERATION
			"	rjmp .+0		\n\t"	// C: 2		9	| JUMP 1 FOWARD
			"	rjmp .+0		\n\t"	// C: 2		11	| JUMP 1 FOWARD
			"	rjmp .+0		\n\t"	// C: 2		13	| JUMP 1 FOWARD
			"		out	%2,%4	\n\t"	// C: 1		15	| SET ws2812_portreg == MASKLO - '1' [+1] '0' [+1] - fe-high
			"	rjmp .+0		\n\t"	// C: 2		16	| JUMP 1 FOWARD
			"		dec	%0		\n\t"	// C: 1		18	| SET CTR -= 1 - '1' [+2] '0' [+2]
			"		brne loop%=	\n\t"	// C: 2		19	| JUMP TO LOOP IF Z ( = ctr ) == 0 - '1' [+3] '0' [+4]
			:	"=&d" (ctr)
			:	"r" (curbyte),
				"I" (_SFR_IO_ADDR(ws2812_PORTREG)),
				"r" (maskhi),
				"r" (masklo)
		);
	}
	
	SREG=sreg_prev;
}
