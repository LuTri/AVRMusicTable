#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/boot.h>

#include "OdroidUart/basic/setup.h"
#include "fncs/functionalities.h"
#include "modes.h"

int main(void) {
    uart_setup();
    sei();

    main_loop();
}
