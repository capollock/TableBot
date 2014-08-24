#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include "cInfra.h"

int main()
{
	uint16_t leftReading = 0;
	uint16_t rightReading =0;
	
	initUSART();
	initInfraADC();
	while(1)
	{
		leftReading = getReading(LEFT_INFRA_CH);
		rightReading = getReading(RIGHT_INFRA_CH);
		printString("L: ");
		_delay_ms(300);
		printWord(leftReading);
		_delay_ms(300);
		printString("R: ");
		_delay_ms(300);
		printWord(rightReading);
		_delay_ms(1000);
	}
	return 0;
}
