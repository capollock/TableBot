#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include "cSonic.h"
#include "cServo.h"
#include "cProgMem.h"		// Source of Look Up Table in Flash

int main(void)
{
	initUSART();
	initSonic();
	initServoOC1A();
	DDRB |= (1 << PB1);
	OCR1A = pgm_read_word(pulseLenArr + POS_INDEX_2);
	
	while(1)
	{
		debugPrint("Sonic1: ",  ultraSonic1());
		debugPrint("Sonic2: ", ultraSonic2());
		_delay_ms(1000);
	}

	return 0;
}