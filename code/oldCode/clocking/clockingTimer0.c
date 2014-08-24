#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"


void initTimer0(void)
{
	TCCR0A = 0;
	TCCR0B |= (1 << CS02);
}

int main(void)
{
	initTimer0();
	initUSART();
	DDRD = 0;
	PORTD |= (1 << PD2);
	printString("----- Initiating Test-----\n\r");
	TCNT0 = 0;
	while(1)
	{
		if(PIND & (1<<PD2))
		{

		}
		else
		{
			printByte(TCNT0);
			printString("\n\r");
		}
		_delay_us(5);
	}	
	return 0;
}