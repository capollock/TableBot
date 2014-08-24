#include <avr/io.h>
#include "cMotor.h"

void initMotorAB(void)
{
	//Set Data Direction Registers
	MOTOR_A_EN_DDR |= (1 << MOTOR_A_EN);
	MOTOR_A_1_DDR |= (1 << MOTOR_A_1);
	MOTOR_A_2_DDR |= (1 << MOTOR_A_2);
	
	MOTOR_B_EN_DDR |= (1 << MOTOR_B_EN);
	MOTOR_B_1_DDR |= (1 << MOTOR_B_1);
	MOTOR_B_2_DDR |= (1 << MOTOR_B_2);
	
	//Set Timer0	
	//Fast PWM - Non-Inverting - Timer clock prescaling by 8
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1); 
	TCCR0A |= (1 << WGM00) | (1 << WGM01);
	TCCR0B |= (1 << CS01);
}

void setSpeedA(uint8_t speed)
{
	OCR0A = speed;
}

void setSpeedB(uint8_t speed)
{
	OCR0B = speed;
}

void directionA(uint8_t dir)
{
	if(dir ==1)
	{
		//Both Low
		MOTOR_A_1_PORT &= ~(1 << MOTOR_A_1);
		MOTOR_A_2_PORT &= ~(1 << MOTOR_A_2);
	}
	else if(dir ==2)
	{
		//ONE_A High ONE_B Low
		MOTOR_A_1_PORT |= (1 << MOTOR_A_1);
		MOTOR_A_2_PORT &= ~(1 << MOTOR_A_2);
	}
	else if (dir == 3)
	{
		//ONE_B High ONE_A Low
		MOTOR_A_1_PORT &= ~(1 << MOTOR_A_1);
		MOTOR_A_2_PORT |= (1 << MOTOR_A_2);
	}
	else if (dir==4)
	{
		//Both High
		//Both Low
		MOTOR_A_1_PORT |= (1 << MOTOR_A_1);
		MOTOR_A_2_PORT |= (1 << MOTOR_A_2);
	}
}

void directionB(uint8_t dir)
{
	if(dir ==1)
	{
		//Coast
		//Both Low
		MOTOR_B_1_PORT &= ~(1 << MOTOR_B_1);
		MOTOR_B_2_PORT &= ~(1 << MOTOR_B_2);
	}
	else if(dir ==2)
	{
		//Backward
		//ONE_A High ONE_B Low
		MOTOR_B_1_PORT |= (1 << MOTOR_B_1);
		MOTOR_B_2_PORT &= ~(1 << MOTOR_B_2);
	}
	else if (dir == 3)
	{
		//Forward
		//ONE_B High ONE_A Low
		MOTOR_B_1_PORT &= ~(1 << MOTOR_B_1);
		MOTOR_B_2_PORT |= (1 << MOTOR_B_2);
	}
	else if (dir==4)
	{
		//Brake
		//Both High
		MOTOR_B_1_PORT |= (1 << MOTOR_B_1);
		MOTOR_B_2_PORT |= (1 << MOTOR_B_2);
	}
}

void drive(uint8_t command, uint8_t speedA, uint8_t speedB)
{
	switch(command)
	{
		//brake
		case 0:
		{
			directionA(4);
			directionB(4);
			setSpeedA(speedA);
			setSpeedB(speedB);			
			break;
		}
		//FORWARD
		case 1:
		{
			directionA(3);
			directionB(3);
			setSpeedA(speedA);
			setSpeedB(speedB);
			break;
		}
		//BACKWARD
		case 2:
		{
			directionA(2);
			directionB(2);
			setSpeedA(speedA);
			setSpeedB(speedB);
			break;
		}
		//COAST
		case 3:
		{
			directionA(1);
			directionB(1);
			setSpeedA(speedA);
			setSpeedB(speedB);
			break;
		}
		//PIVOT_RIGHT
		case 4:
		{
			directionA(2);
			directionB(3);
			setSpeedA(speedA);
			setSpeedB(speedB);
			break;
		}
		//PIVOT_LEFT
		case 5:
		{
			directionA(3);
			directionB(2);
			setSpeedA(speedA);
			setSpeedB(speedB);
			break;
		}
	}
}
