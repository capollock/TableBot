#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"


//------------------------------------------------------------------

int main(void)
{
	initUSART();
	while(1)
	{
		printString("Testing");
		_delay_ms(1000);
	}
	return 0;
}


