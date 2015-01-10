#ifndef _HOSTCOM_H
#define _HOSTCOM_H

void uart_init(void);
void uart_putc(unsigned char c);
void uart_puts(char* s);
uint8_t uart_getc(void);
void uart_gets(char* Buffer, uint8_t MaxLen);


#endif
