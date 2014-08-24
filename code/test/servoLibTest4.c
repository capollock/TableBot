#include <avr/io.h>
#include <util/delay.h>
#include "cServo.h"
#include "USART.h"

#define 	DELAY_TIME 			4000

/*------------------------------------------------------------------
Move to Motor.h
------------------------------------------------------------------*/
#define 	EN_ONE_TWO 			PD5
#define 	EN_ONE_TWO_PORT		PORTD
#define 	EN_ONE_TWO_DDR		DDRD

#define 	ONE_A				PD4
#define 	ONE_A_PORT			PORTD
#define		ONE_A_DDR			DDRD

#define 	TWO_A				PD3
#define 	TWO_A_PORT			PORTD
#define		TWO_A_DDR			DDRD

#define 	EN_THREE_FOUR		PD6
#define		EN_THREE_FOUR_DDR	DDRD
#define		EN_THREE_FOUR_PORT	PORTD

#define 	ONE_B				PD7
#define 	ONE_B_PORT			PORTD
#define		ONE_B_DDR			DDRD

#define 	TWO_B				PB0
#define		TWO_B_PORT			PORTB
#define		TWO_B_DDR			DDRB

void initMotorAB(void)
{

	
	//Set Timer0	
	//Fast PWM - Non-Inverting - Timer clock prescaling by 8
	TCCR0A |= (1 << COM0A1); //| (1 << COM0B1); 
	TCCR0A |= (1 << WGM00) | (1 << WGM01);
	TCCR0B |= (1 << CS01);
}

void setSpeedA(uint8_t speed)
{
	OCR0A = speed;
}

void directionA(uint8_t dir)
{
	if(dir ==1)
	{
		//Both Low
		ONE_A_PORT &= ~(1 << ONE_A);
		ONE_B_PORT &= ~(1 << TWO_A);
	}
	else if(dir ==2)
	{
		//ONE_A High ONE_B Low
		ONE_A_PORT |= (1 << ONE_A);
		ONE_B_PORT &= ~(1 << TWO_A);
	}
	else if (dir == 3)
	{
		//ONE_B High ONE_A Low
		ONE_A_PORT &= ~(1 << ONE_A);
		ONE_B_PORT |= (1 << TWO_A);
	}
	else if (dir==4)
	{
		//Both High
		//Both Low
		ONE_A_PORT |= (1 << ONE_A);
		ONE_B_PORT |= (1 << TWO_A);
	}
}

void setSpeedB(uint8_t speed)
{
	OCR0B = speed;
}

//------------------------------------------------------------------

int main(void)
{
	DDRB |= (1 << PB5);
	PORTB |= (1 << PB5);
	
	
	//Set Data Direction Registers
	EN_ONE_TWO_DDR |= (1 << EN_ONE_TWO);
	ONE_A_DDR |= (1 << ONE_A);
	TWO_A_DDR |= (1 << TWO_A);
	EN_THREE_FOUR_DDR |= (1 << EN_THREE_FOUR);
	ONE_B_DDR |= (1 << ONE_B);
	TWO_B_DDR |= (1 << TWO_B);
	
	uint8_t num;
	initUSART();
	initMotorAB();
	
	//initServoOC1A();
	while(1)
	{
		ONE_A_PORT |= (1 << ONE_A);
		ONE_B_PORT &= ~(1 << ONE_B);
		num = getNumber();
		//tredEyeControl(num);
		//directionA(num);
		printString("num: ");
		printByte(num);
		printString("\r\n");
		setSpeedA(num);
		_delay_ms(4000);
	}
	return 0;
}


