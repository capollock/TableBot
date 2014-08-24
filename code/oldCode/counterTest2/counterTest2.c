/*
	This example uses Timer0 to toggle PD6 / OC0A and print 'Triggered' once there is an overflow in the CTC register
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART.h"

#define LED_PIN	PD6

ISR(TIMER0_COMPA_vect)
{
	printString("triggered");
}

int main(void)
{
	DDRD |= 0xff; 								//Set LED_PIN to Output
	TCCR0B |= ((1 << CS02) | (1 << CS00)); 		//Set clock pre-scale for Timer0 to divide by 1024
	TCCR0A |= (1 << WGM01);                    	//Important to set CTC mode if using a compare based interrupt
	TCCR0A |= (1 << COM0A0);					//Toggle Pin on Overflow
	OCR0A = 254;								//Set the Overflow Compare Register an 8 bit number
	
	
	TIMSK0 |= (1 << OCIE0A);	//Enable Compare Match A Interrupt
	TIMSK0 |= (1 << TOIE0);     //Enable Overflow Interrupt
	sei();						//Enable Interrupts					
	initUSART();				//Enable Serial
	
	uint8_t i = 0; 
	
	while(1)
	{
		_delay_ms(1000);
		printString("Delay");
	}

}