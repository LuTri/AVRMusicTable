#include <avr/io.h>
#include "host.h"

#define WATCHDOG_PRESCALER ((1<<CS02) | (1<<CS01))
 
#define BAUD 9600UL			// Baudrate
 
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)	 // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))		 // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.
 
#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
	#error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch! 
#endif

#define UART_CONT 1
#define UART_SUCC 2
#define UART_ERR 3

volatile uint8_t UART_FLAGS = 0;

/* UART WATCHDOG */ 
ISR(TIMER0_OVF_vect) {
	UART_FLAGS &= ~(1<<UART_CONT);
}

inline void reset_watchdog(void) {
	TCNT0 = 0;
}

void start_watchdog(void) {
	UART_FLAGS |= (1<<UART_CONT);
	TIMSK0 |= (1<<TOIE0);
	TCCR0B |= WATCHDOG_PRESCALER;
	sei();
}

void stop_watchdog(void) {
	cli();
	reset_watchdog();
	UART_FLAGS &= ~(1<<UART_CONT);
	TIMSK0 &= ~(1<<TOIE0);
	TCCR0B &= ~(WATCHDOG_PRESCALER);
}

void uart_init(void) {
	UBRR0 = UBRR_VAL;
	UCSR0B |= (1<<TXEN0) | (1<<RXEN0);
	// Frame Format: Asynchron 8N1
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

inline uint8_t uart_available(void) {
	return (UCSR0A & (1<<RXC0)) && (UART_FLAGS & (1<<UART_CONT));
}

void uart_putc(unsigned char c) {
		/* warten bis sende möglich */
		while (!(UCSR0A & (1<<UDRE0))) {}
		UDR0 = c;
}

void uart_puts(char* s) {
	while (*s) {
		uart_putc(*s);
		s++;
	}
}

uint8_t uart_getc(void) {
	while (!(uart_available()));
	return UDR0;
}

void uart_gets(char* Buffer, uint8_t MaxLen) {
	uint8_t NextChar;
	uint8_t StringLen = 0;
 
	NextChar = uart_getc();
 
	while( NextChar != '\n' && StringLen < MaxLen - 1 && NextChar != '\0') {
		*Buffer++ = NextChar;
		StringLen++;
		NextChar = uart_getc();
	}
	*Buffer = '\0';
}

void read_uart(uint8_t* leds) {
	uint8_t lengthH,lengthL,buff;
	uint16_t datlen;
	
	lengthH = uart_getc();
	lengthL = uart_getc();

	datlen = (lengthH << 8) + lengthL;
	while (datlen--) {
		*leds++ = uart_getc();
	}

	while (uart_available()) {
		buff = uart_getc();
	}
}
