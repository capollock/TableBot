#include <avr/io.h>
#include <util/delay.h>

#define BUTTON_IN PD2	
#define BUTTON_PIN PIND
#define LED_OUT	PB1

int main(void)
{
	DDRD &= ~(1 << BUTTON_IN); // Confirm default input
	PORTD |= (1 << BUTTON_IN);  // Enable pull-up resistor
	

	DDRB |= (1 << LED_OUT); // Set LED_OUT to output
	for (uint8_t count=0; count < 10; count++)
	{	
		PORTB |= (1 << LED_OUT);// On
		_delay_ms(250); 	// Wait
		PORTB &= ~(1 << LED_OUT); // Off
		_delay_ms(250);		  // Wait
	}
	
	while(1)
	{
		if(bit_is_clear(PIND, PD2))
		{
						
			PORTB ^= (1 << LED_OUT);
			_delay_us(1000);
		}
	}
	return 0;
}