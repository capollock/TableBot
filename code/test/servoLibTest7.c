#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include "cServo.h"
#include "cSonic.h"
#include "cInfra.h"


static inline uint16_t getNumber16(void);

int main(void)
{
	//Led On -------------------------------------------------------
	DDRB |= (1 << PB5);
	PORTB |= (1 << PB5);
	//--------------------------------------------------------------
	
	//Initialization -----------------------------------------------
	initUSART();
	initServoOC1A();
	initSonic();
	initInfraADC();
	//--------------------------------------------------------------
	
	//Variables ----------------------------------------------------
	uint16_t pulseLength = 0;
	int leftReading = 0;
	int rightReading = 0;
	uint16_t infraLeft = 0;
	uint16_t infraRight = 0;
	//--------------------------------------------------------------
	
	while(1)
	{
		if (bit_is_set(UCSR0A, RXC0))
		{	
			pulseLength = getNumber16();
			printString("Pulse length: ");
			printWord(pulseLength);
			printString("\n\r");
			DDRB |= (1 << PB1);   
			OCR1A = pulseLength;
		}
		else
		{
			printString("No bits \n\r");
		}
		leftReading = ultraSonic1();
		printString("Left Sonic: ");
		printWord(leftReading);
		printString("\n\r");
		_delay_ms(100);
		rightReading = ultraSonic2();
		printString("Right Sonic: ");
		printWord(rightReading);
		printString("\n\r");
		_delay_ms(100);
		printString("Left Infra: ");
		infraLeft = getReading(LEFT_INFRA_CH);
		printWord(infraLeft);
		printString("\n\r");
		_delay_ms(100);
		leftReading = ultraSonic2();
		printString("Right Infra: ");
		infraRight = getReading(RIGHT_INFRA_CH);
		printWord(infraRight);
		printString("\n\r");
		_delay_ms(1000);
	}
	return 0;
}


static inline uint16_t getNumber16(void) {
  // Gets a PWM value from the serial port.
  // Reads in characters, turns them into a number
  char thousands = '0';
  char hundreds = '0';
  char tens = '0';
  char ones = '0';
  char thisChar = '0';

  do {
    thousands = hundreds;                        /* shift numbers over */
    hundreds = tens;
    tens = ones;
    ones = thisChar;
    thisChar = receiveByte();                   /* get a new character */
    transmitByte(thisChar);                                    /* echo */
  } while (thisChar != '\r');

  transmitByte('\n');                                       /* newline */
  return (1000 * (thousands - '0') + 100 * (hundreds - '0') +
          10 * (tens - '0') + ones - '0');
}

