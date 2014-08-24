#include <avr/io.h>
#include <util/delay.h>
#include "cMotor.h"
#include "USART.h"


//------------------------------------------------------------------

int main(void)
{
	DDRB |= (1 << PB5);
	PORTB |= (1 << PB5);
	
	initMotorAB();
	initUSART();
	uint8_t num = 0;
	
	//initServoOC1A();
	while(1)
	{
		//MOTOR_A_1_PORT |= (1 << MOTOR_A_1);
		//MOTOR_A_2_PORT &= ~(1 << MOTOR_A_2);
		//num = getNumber();
		drive(1, 150,150);
		_delay_ms(5000);
		drive(2, 150, 150);
		_delay_ms(5000);
	}
	return 0;
}


