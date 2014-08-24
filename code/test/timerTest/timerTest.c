/* -----------------------------------------------------------------
   Routine to Use Timer And Interrupt
----------------------------------------------------------------- */

//Include AVR libraries
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART.h"

static inline void initTimer1(void)
{
	TCCR1B |= (1 << CS11) | (1 << CS10);  
}

int main(void)
{
	//--------Initialize Hardware-------------------
	initUSART();
	initTimer1();
	
	DDRB = 0xff;
	DDRD = 0x0;
	PORTD |= (1 << 2);
	
	PORTB = 0xff;
	_delay_ms(1000);
	PORTB = 0x0;
	uint8_t byte;
	
	while(1)
	{
		TCNT1 = 0;
		printString("Press any button to continue\r\n");	
		byte = receiveByte();
		_delay_ms(100);
		PORTB |= (1 << 1);
		loop_until_bit_is_clear(PIND, 2);
		printByte(TCNT1>>4);
		printString("\r\n");
		PORTB &= ~(1 << 1);
	}
	
}
