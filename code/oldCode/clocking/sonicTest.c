#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include "cSonic.h"

int main(void)
{
	initUSART();
	initSonic();
	int reading = 0;
	while(1)
	{
		reading = ultraSonic2();
		printString("Sonic 2: ");
		printWord(reading);
		printString("\n\r");
		_delay_ms(500);
		reading = ultraSonic1();
		printString("Sonic 1: ");
		printWord(reading);
		printString("\n\r");
		_delay_ms(500);
		}
	return 0;
}



