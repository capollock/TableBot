#include <avr/io.h>
#include <util/delay.h>
#include "cServo.h"
#include "USART.h"

#define DELAY_TIME 4000

int main(void)
{
	DDRB &= ~(1 << PB1);
	PORTB &= ~(1 << PB1);
	DDRB |= (1 << PB5);
	PORTB |= (1 << PB5);
	uint16_t num2 = 0;
	uint8_t num = 0;
	uint8_t num1 = 0;
	initUSART();
	initServoOC1A();
	
	while(1)
	{
		OC01A = receiveByte();
		printByte(num2);
		printString("Done!");
		printString("\n\r");
		_delay_ms(200);
	}
	return 0;
}


