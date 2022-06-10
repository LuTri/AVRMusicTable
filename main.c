#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/boot.h>
#include <util/delay.h>

#include "OdroidUart/basic/setup.h"
#include "fncs/functionalities.h"
#include "modes.h"
#include "display/ws2812.h"

void reset(void) {
    for (uint8_t idx = 0; idx < 112; idx++) {
        set_led(idx, 0, 0, 0);
    }
    ws2812_setleds();
    _delay_ms(500);

}

int main(void) {
    //uart_setup();
    //sei();
    //
    DDRD |= (1 << PD5);
    DDRB |= (1 << PB5);

    reset();

    set_led(0, 20,20, 20);
    set_led(1, 220, 220, 220);

    ws2812_pwm_out();

    _delay_ms(500);

    reset();

    set_led(0, 255, 0, 0);
    //set_led(1, 220, 220, 220);

    ws2812_pwm_out();


    while(1) {
        ;;
    }

    main_loop();
}
