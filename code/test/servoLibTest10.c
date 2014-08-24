#include <avr/io.h>
#include <util/delay.h>
#include "USART.h" 			// serial Comm for Debugging
#include "cServo.h"			// Library for Controlling Servo
#include "cSonic.h"			// Library for Ultra Sonic 
#include "cInfra.h"			// Library for Infrared 
#include "cMotor.h"			// Library for Motor
#include "cProgMem.h"		// Source of Look Up Table in Flash

// ------------------------------------------------------------------
// ----- Global Variables -------------------------------------------

uint16_t rightSonicThresh = 0;
uint16_t leftSonicThresh = 0;

// ------------------------------------------------------------------

// ----- Functions --------------------------------------------------
void toggleLed(void);

// ------------------------------------------------------------------

int main(void)
{
	//Led On -------------------------------------------------------
	DDRB |= (1 << PB5);
	PORTB |= (1 << PB5);
	//--------------------------------------------------------------
	
	//Initializations ----------------------------------------------
	initServoOC1A();
	initUSART();
	initSonic();
	initInfraADC();
	initMotorAB();
	//--------------------------------------------------------------
	
	//Local Variables ----------------------------------------------
	uint8_t i = 0;	
	uint8_t position_one = 1;
	uint8_t pos_one_cnt=0;
	uint8_t leftSonicReading = 0;
	uint8_t rightSonicReading = 0;
	uint16_t leftInfraReading = 0;
	uint16_t rightInfraReading = 0;
	
	//--------------------------------------------------------------
	
	toggleLed(); //Signal Initialization Warning 
	
	//Move to Servo position 1--------------------------------------
	DDRB |= (1 << PB1);
	OCR1A = pgm_read_word(pulseLenArr + POS_INDEX_1); //Place Servo In Down Position
	_delay_ms(40); // Offload to timer later
	DDRB &= ~(1 << PB1); //Stop Clicking
	
	//Servo position 1 Expected Distance ---------------------------
	leftSonicReading = pgm_read_word(lSonicLookUp + POS_INDEX_1);
	rightSonicReading = pgm_read_word(rSonicLookUp + POS_INDEX_1);
		
	while(position_one)
	{
		for(pos_one_cnt=0;pos_one_cnt<100; pos_one_cnt = pos_one_cnt)
		{
			//Sonic Sensor Reading---------------------------
			leftSonicThresh = ultraSonic1();
			rightSonicThresh = ultraSonic2();		
			leftInfraReading = getReading(LEFT_INFRA_CH);
			rightInfraReading = getReading(RIGHT_INFRA_CH);
			
			if(leftSonicThresh - leftSonicReading < 10 || leftSonicThresh - leftSonicReading > 65530)			
			{
				if(rightSonicThresh - rightSonicReading < 10 || rightSonicThresh - rightSonicReading > 65530)
				{
				
					if(leftInfraReading < 950 && rightInfraReading < 950)
					{
						pos_one_cnt++;
						printString("Pos_one_cnt: ");
						printWord(pos_one_cnt);
						printString("\n\r");
					}
				}
				//Debug
				else
				{
					pos_one_cnt = 0;
				}
			}
			else
			{
				pos_one_cnt = 0;
			}
		
		_delay_ms(10);
		printString("Right infra");
		printWord(rightInfraReading);
		printString("\n\r");
		printString("Left infra");
		printWord(leftInfraReading);
		printString("\n\r");
		}
		position_one = 0;
	}
	
	DDRB |= (1 << PB1);
	OCR1A = pgm_read_word(pulseLenArr + POS_INDEX_3);
	_delay_ms(1000);
	OCR1A = pgm_read_word(pulseLenArr + POS_INDEX_2);
	
	leftSonicThresh = pgm_read_word(lSonicLookUp+POS_INDEX_2);
	rightSonicThresh = pgm_read_word(rSonicLookUp+POS_INDEX_2);
	
	drive(4, 220, 220);
	
	while(1)
	{
		printString("loop");
		_delay_ms(1000);
	}
	return 0;
}

void toggleLed(void)
{
	uint8_t i = 0;	
	for (i=0; i < 8; i++)
	{
		PORTB ^= (1 << PB5);
		_delay_ms(100);
	}
	PORTB |= (1 << PB5);
}