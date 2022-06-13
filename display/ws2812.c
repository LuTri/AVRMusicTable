#include "ws2812.h"

#ifndef UNITTEST
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#if WS2812_PAUSE_INTERRUPTS
#include "../OdroidUart/commands.h"
#endif

#else
#include "unittest.h"
#endif

uint8_t block_output = 0;


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

uint8_t ws2812_sendarray_mask(void);
 
uint8_t ws2812_setleds(void) {
    uint8_t _err = 0;
    ws2812_DDRREG |= ws2812_pin; // Enable DDR
    _err = ws2812_sendarray_mask();
    _delay_us(50);

    return _err;
}

uint8_t asm_ws2812_out(uint8_t** data, uint8_t maskhi) {

    uint8_t masklo, ctr, curbyte;
    uint16_t datlen = N_LEDS / WS2812_SPLIT;

    masklo    =~maskhi&ws2812_PORTREG;

    if (block_output) {
        return 1;
    }

#ifndef UNITTEST
    while (datlen--) {
    curbyte=*(*data)++;

        asm volatile(
            "    ldi %0,8      \n\t"  // Set ctr to 8
            "loop%=:           \n\t"
            "      out %2,%3   \n\t"  // Out 1 to PINS
            "    nop           \n\t"
            "    rjmp .+0      \n\t"  // Cycle waste
            "    sbrs %1,7     \n\t"  // Skip if bit 7 set
            "      out %2,%4   \n\t"  // Out 0 to PINS
            "    lsl %1        \n\t"  // left shift curbyte
            "    nop           \n\t"
            "    rjmp .+0      \n\t"  // Cycle waste
            "    rjmp .+0      \n\t"  // Cycle waste
            "    rjmp .+0      \n\t"  // Cacle waste
            "    out %2,%4     \n\t"  // Out 0 to PINS
            "    rjmp .+0      \n\t"  // Cycle waste
            "      dec %0      \n\t"  // Decrement ctr
            "      brne loop%= \n\t"  // back to loop
            :    "=&d" (ctr)
            :    "r" (curbyte),
                 "I" (_SFR_IO_ADDR(ws2812_PORTREG)),
                 "r" (maskhi),
                 "r" (masklo)
        );
    }
#endif /* ifndef UNITTEST */
    return 0;
}

inline uint8_t ws2812_sendarray_mask(void) {
    uint8_t* data = (uint8_t*)(leds);
    uint8_t err = 0;
#if WS2812_PAUSE_INTERRUPTS
    uint8_t sreg_prev;
#endif

#ifdef WS2812_USE_GAMMA_CORRECTION
    for (uint16_t idx = 0; idx < N_LEDS; idx++) data[idx] = gamma8[data[idx]];
#endif

#if WS2812_PAUSE_INTERRUPTS
    /* Don't get interrupted, but also don't mess up time critical processes */
    sreg_prev=SREG;
    cli();
#endif

    err |= asm_ws2812_out(&data, (1 << PB4));
    err |= asm_ws2812_out(&data, (1 << PB3));
    err |= asm_ws2812_out(&data, (1 << PB2));
    err |= asm_ws2812_out(&data, (1 << PB1));

#if WS2812_PAUSE_INTERRUPTS
    SREG=sreg_prev;
#endif

    return err;
}
