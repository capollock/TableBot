#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART.h"

#define LED_PIN	PD6

int main(void)
{
	DDRD |= 0xff; 		//Set LED_PIN to Output
	TCCR0B |= ((1 << CS02) | (1 << CS00)); 		//Set clock pre-scale for Timer0 to divide by 1024
	TCCR0A |= (1 << WGM01);                                  /* CTC mode */
	TCCR0A |= (1 << COM0A0);
	OCR0A = 254;
	
	
	TIMSK0 |= (1 << OCIE0A);	//Enable Compare Match A Interrupt
	TIMSK0 |= (1 << TOIE0);     //Enable Overflow Interrupt
	sei();						//Enable Interrupts					//Each Tick of Timer is roughly 1 second
	initUSART();
	
	uint8_t i = 0; 
	
	while(1)
	{
		_delay_ms(100);
		if (i < 255)
		{
			i = 0;
		}
		else
		{
			i = i + 1;
		}
		printString(i);
		printString("next \n\r");
	}

}