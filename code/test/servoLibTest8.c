#include <avr/io.h>
#include <util/delay.h>
//#include <avr/pgmspace.h>
#include "USART.h"
#include "cServo.h"
#include "cProgMem.h"

int main(void)
{
	//Led On -------------------------------------------------------
	DDRB |= (1 << PB5);
	PORTB |= (1 << PB5);
	//--------------------------------------------------------------
	
	//Initialization -----------------------------------------------
	initServoOC1A();
	initUSART();
	//--------------------------------------------------------------
	
	//Variables ----------------------------------------------------
	uint8_t i = 0;	

	//--------------------------------------------------------------
	
	while(1)
	{
		if(i==9){i=0;}
		printWord(pgm_read_word(pulseLenArr+i));
		printString("Space \n\r");
		printWord(pgm_read_word(lServoLookUp+i));
		printString("Space \n\r");
		printWord(pgm_read_word(rServoLookUp+i));
		
		printString("Space \n\r");
		_delay_ms(3000);
		i++;
	}
	return 0;
}

