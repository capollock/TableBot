#include <avr/io.h>
#include <util/delay.h>
#include "cServo.h"

int main(void)
{
	initServoOC1A();
	while(1)
	{
		angleToDutyCycle(90);
		_delay_ms(1000);
		angleToDutyCycle(1);
		_delay_ms(1000);
		angleToDutyCycle(180);
		_delay_ms(1000);
	}
	return 0;
}


