#ifndef _HOSTCOM_H
#define _HOSTCOM_H

#include <avr/io.h>

void uart_init(void);
void uart_putc(unsigned char c);
void uart_puts(char* s);
uint8_t uart_getc(void);
void uart_gets(char* Buffer, uint8_t MaxLen);
uint8_t uart_available(void);
uint8_t read_uart(uint8_t* leds);

void writefloat(char* buf, float val);

volatile uint8_t UART_FLAGS;

#endif
