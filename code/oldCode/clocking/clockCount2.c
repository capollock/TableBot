#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART.h"

volatile int counter = 0;

void initTimer0(void)
{
	//Compare Output Mode: Normal Mode (by Default)
	//Wave Generation Mode: CTC Mode
	TCCR0A |= (1 << WGM01);
	//Clock Select: 1024 prescaler
	TCCR0B |= (1 << CS02) | (1 << CS00);
	//Timer Interrput Mask Reg: Enable Interrupt on Compare Match A
	TIMSK0 |= (1 << OCIE0A);
	//Enable Interrupts Globally
	sei();
	//Set Compare Match A to 100 (i.e. every 100,024 us)
	OCR0A = 100;	
}

ISR(TIMER0_COMPA_vect)
{
	counter += 1;
	PORTB ^= (1 << PB2);
}

int main(void)
{
	initTimer0();
	cli();
	initUSART();
	DDRB |= (1 << PB2);
	PORTB |= (1 << PB2);
	_delay_ms(5000);
	PORTB &= ~(1 <<PB2);
	TCNT0 = 0;
	sei();
	while(1)
	{
		_delay_ms(100);
		printByte(counter);
		printString("\n\r");
	}
}