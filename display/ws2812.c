#include "ws2812.h"

#ifndef UNITTEST
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#else
#include "unittest.h"
#endif


#ifdef WS2812_USE_GAMMA_CORRECTION
static const __flash uint8_t gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };
#endif

cRGB leds[N_PACKS];

void set_led(uint8_t idx, uint8_t r, uint8_t g, uint8_t b) {
    leds[idx].r = r;
    leds[idx].g = g;
    leds[idx].b = b;
}

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
#if WS2812_PAUSE_INTERRUPTS
	uint8_t sreg_prev;
#endif

	masklo	=~maskhi&ws2812_PORTREG;
	maskhi |=		ws2812_PORTREG;

#if WS2812_PAUSE_INTERRUPTS
	sreg_prev=SREG;
	cli();
#endif

#ifndef UNITTEST
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
#ifdef WS2812_USE_GAMMA_CORRECTION
			:	"r" (gamma8[curbyte]),
#else
			:	"r" (curbyte),
#endif
				"I" (_SFR_IO_ADDR(ws2812_PORTREG)),
				"r" (maskhi),
				"r" (masklo)
		);
	}
#endif /* ifndef UNITTEST */

#if WS2812_PAUSE_INTERRUPTS
	SREG=sreg_prev;
#endif
}
