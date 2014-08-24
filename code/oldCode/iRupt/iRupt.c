//------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//------------------------------------------------------------------
// Definitions and Global Variables
//------------------------------------------------------------------

#define DELAY_TIME	1000
uint8_t led_one = 0;
uint8_t led_two = 1;
 
//------------------------------------------------------------------
// Routines
//------------------------------------------------------------------

ISR(INT0_vect)
{
	PORTB ^= (1 << 3);
}

void initInterrupt0(void)
{
	EIMSK |= (1 <<INT0);
	EICRA |= (1 << ISC00);
	sei();
}

int main(void)
{
	DDRB = 0xff;
	DDRD = 0x0;
	PORTB = 0xff;
	_delay_ms(DELAY_TIME);
	PORTB = 0x0;
	PORTD |= (1 << 2);
	_delay_ms(DELAY_TIME);
	initInterrupt0();
	
	while(1)
	{
		PORTB ^= (1 << led_one);
		PORTB ^= (1 << led_two);
		_delay_ms(1000);
	}

	return (0);
}