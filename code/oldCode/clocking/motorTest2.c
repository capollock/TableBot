#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"

#define 	EN_ONE_TWO 		PD5
#define 	ONE_A			PD4
#define 	TWO_A			PD3

void initTimer0A(void)
{
	//Fast PWM - Non-Inverting - Timer clock prescaling by 8
	TCCR0A |= (1 << COM0B1) | (1 << WGM01); 
	TCCR0A |= (1 << WGM00);
	TCCR0B |= (1 << CS01);
}

void setSpeed(uint8_t speed)
{
	OCR0B = speed;
}

int main(void)
{
	initTimer0A();
	initUSART();
	//Set PD7 and PD6 Pins for Output
	DDRD |= (1 << ONE_A) | (1 << EN_ONE_TWO);
	//Set PB0 for Output
	DDRD |= (1 << TWO_A);
	PORTD &= ~(1 << TWO_A);
	PORTD |= (1 << ONE_A);
	OCR0A =0;
	while(1)
	{
		uint8_t num = getNumber();
		printByte(num);
		setSpeed(num);
	}
}
