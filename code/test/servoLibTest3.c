#include <avr/io.h>
#include <util/delay.h>
#include "cServo.h"
#include "USART.h"

#define DELAY_TIME 4000

#define 	EN_ONE_TWO 		PD5
#define 	ONE_A			PD4
#define 	TWO_A			PD3
#define 	EN_THREE_FOUR	PD6
#define 	ONE_B			PD7
#define 	TWO_B			PB0


void initMotorPWM(void)
{
	//Fast PWM - Non-Inverting - Timer clock prescaling by 8
	TCCR0A |= (1 << COM0B1) | (1 << WGM01) | (1 << COM0A1); 
	TCCR0A |= (1 << WGM00);
	TCCR0B |= (1 << CS01);
}

void setSpeedA(uint8_t speed)
{
	OCR0B = speed;
}

void setSpeedB(uint8_t speed)
{
	OCR0A = speed;
}


int main(void)
{
	DDRB &= ~(1 << PB1);
	PORTB &= ~(1 << PB1);
	DDRB |= (1 << PB5);
	PORTB |= (1 << PB5);
	uint8_t num = 0;
	initUSART();
	initServoOC1A();
	
	while(1)
	{
		num = getNumber();
		tredEyeControl(num);
		printString("num: ");
		printByte(num);
		printString("\r\n");
		_delay_ms(100);
	}
	return 0;
}


