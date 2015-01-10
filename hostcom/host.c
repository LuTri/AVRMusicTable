#include <avr/io.h>
#include "host.h"

/* 
UART-Init: 
Berechnung des Wertes für das Baudratenregister 
aus Taktrate und gewünschter Baudrate
*/
 
#ifndef F_CPU
/* In neueren Version der WinAVR/Mfile Makefile-Vorlage kann
	 F_CPU im Makefile definiert werden, eine nochmalige Definition
	 hier wuerde zu einer Compilerwarnung fuehren. Daher "Schutz" durch
	 #ifndef/#endif 
 
	 Dieser "Schutz" kann zu Debugsessions führen, wenn AVRStudio 
	 verwendet wird und dort eine andere, nicht zur Hardware passende 
	 Taktrate eingestellt ist: Dann wird die folgende Definition 
	 nicht verwendet, sondern stattdessen der Defaultwert (8 MHz?) 
	 von AVRStudio - daher Ausgabe einer Warnung falls F_CPU
	 noch nicht definiert: */
#warning "F_CPU war noch nicht definiert, wird nun nachgeholt mit 4000000"
#define F_CPU 4000000UL	// Systemtakt in Hz - Definition als unsigned long beachten 
												 // Ohne ergeben sich unten Fehler in der Berechnung
#endif
 
#define BAUD 9600UL			// Baudrate
 
// Berechnungen
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)	 // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))		 // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.
 
#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
	#error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch! 
#endif

 
void uart_init(void)
{
	UBRR0 = UBRR_VAL;
	UCSR0B |= (1<<TXEN0) | (1<<RXEN0);
	// Frame Format: Asynchron 8N1
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void uart_putc(unsigned char c)
{
		while (!(UCSR0A & (1<<UDRE0)))	/* warten bis Senden moeglich */
		{
		}														 
 		UDR0 = c;											/* sende Zeichen */
}

void uart_puts(char* s) {
	while (*s)
		{	 /* so lange *s != '\0' also ungleich dem "String-Endezeichen(Terminator)" */
				uart_putc(*s);
				s++;
		}
}

uint8_t uart_getc(void)
{
		while (!(UCSR0A & (1<<RXC0)));	 // warten bis Zeichen verfuegbar
		return UDR0;									 // Zeichen aus UDR an Aufrufer zurueckgeben
}

void uart_gets(char* Buffer, uint8_t MaxLen){
	uint8_t NextChar;
	uint8_t StringLen = 0;
 
	NextChar = uart_getc();				 // Warte auf und empfange das nächste Zeichen
 
																	// Sammle solange Zeichen, bis:
																	// * entweder das String Ende Zeichen kam
																	// * oder das aufnehmende Array voll ist
	while( NextChar != '\n' && StringLen < MaxLen - 1 && NextChar != '\0') {
		*Buffer++ = NextChar;
		StringLen++;
		NextChar = uart_getc();
	}
 
																	// Noch ein '\0' anhängen um einen Standard
																	// C-String daraus zu machen
	*Buffer = '\0';
}
