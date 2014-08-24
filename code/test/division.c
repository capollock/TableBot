#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"

int main()
{
uint8_t loopVar = 0;
double quo = 0;
	initUSART();
	while(1)
	{
		quo = loopVar / 4 * 2;
		loopVar+=1;
		debugPrint("quo: ", quo);
		debugPrint("loopVar: ", loopVar);		
		_delay_ms(500);	
	}
}
