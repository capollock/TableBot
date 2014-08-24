#include <avr/io.h>
#include <util/delay.h>
#include "cServo.h"

#define DEAD_BAND 600
#define SERVO_RANGE 1400

void initServoOC1A(void)
{
	//Set Bits in Timer/Counter Control Register Register for Mode 14	
	
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	
	//Prescale Clock with 1, assuming 1Mghz Clock Speed
	TCCR1B |= (1 << CS10);
	
	//Frequency Set with ICR1 value
	//Duty Cycle Set with OCR1A value in main
	//Non-inverting Mode
	ICR1 = 20000;
	TCCR1A |= (1 << COM1A1);
	//Disable Output on PB1 / OC1A
	DDRB &= ~(1 << PB1);
}

void setDutyCycle(uint16_t timeVal)
{
	if (timeVal <= 0)
	{
		DDRB &= ~(1 << PB1);
	}
	else
	{
		DDRB |= (1 << PB1);
		OCR1A = timeVal;
	}
}

void angleToDutyCycle(uint8_t angle)
{
	DDRB |= (1 << PB1);
	OCR1A = (angle * (SERVO_RANGE / 180)) + DEAD_BAND;
}

void tredEyeControl(uint8_t angle)
{
	if (angle == 0)
	{
		DDRB &= ~(1 << PB1);
	}
	else if (angle < 46)
	{
		DDRB |= (1 << PB1);
		OCR1A = (angle * (SERVO_RANGE / 180)) + DEAD_BAND;
	}
}