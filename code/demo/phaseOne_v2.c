#include <avr/io.h>
#include <util/delay.h>
#include "USART.h" 			// Serial Comm for Debugging
#include "cServo.h"			// Library for Controlling Servo
#include "cSonic.h"			// Library for Ultra Sonic 
#include "cInfra.h"			// Library for Infrared 
#include "cMotor.h"			// Library for Motor
#include "cProgMem.h"		// Source of Look Up Table in Flash

// ----- Function Prototypes ---------------------------------------
void toggleLed(void);
void verifyInfra(void);
void emergencyBrake(uint16_t leftReading, uint16_t rightReading);
uint8_t sensorDelta(uint16_t reading, uint16_t threshold);
// ------------------------------------------------------------------

// ----- Global Variables -------------------------------------------
// None
// ------------------------------------------------------------------

/*
	*****HBRC Phase One*****
	Challenge: Drive to One Side of the Table and Go Back to the Other
	Approach: Four Corners Approach (i.e., keep turning right once sense the edge)
*/
int main(void)
{
	//Led On -------------------------------------------------------
	DDRB |= (1 << PB5);
	PORTB |= (1 << PB5);
	//--------------------------------------------------------------
	
	//Bit Setting --------------------------------------------------
	initServoOC1A();
	initUSART();
	initSonic();
	initInfraADC();
	initMotorAB();
	// -------------------------------------------------------------
	
	// Local variables ---------------------------------------------
	uint8_t leftDelta=0;
	uint8_t rightDelta=0;
	uint16_t leftInfraReading=0;
	uint16_t rightInfraReading=0;
	uint8_t thisTurn=0;
	uint8_t lastTurn=0;
	uint8_t cornerCount=0;
	// -------------------------------------------------------------
	
	//Initialize Routine -------------------------------------------
	toggleLed();
	verifyInfra();
	// -------------------------------------------------------------
	
	// Place servo arm in position ---------------------------------
	DDRB |= (1 << PB1);
	OCR1A = pgm_read_word(pulseLenArr + POS_INDEX_2);
	//--------------------------------------------------------------

	drive(FORWARD, 120, 120);
		
	while(1)
	{
		// Get sensor data -----------------------------------------
		leftInfraReading = getReading(LEFT_INFRA_CH);
		rightInfraReading= getReading(RIGHT_INFRA_CH);
		
		leftDelta = sensorDelta(ultraSonic1(), pgm_read_word(lSonicLookUp+POS_INDEX_2));
		rightDelta = sensorDelta(ultraSonic2(), pgm_read_word(rSonicLookUp+POS_INDEX_2));
		
		
		// Main conditionals  --------------------------------------
		if((leftDelta < 4) && (rightDelta < 4))
		{
			emergencyBrake(leftInfraReading, rightInfraReading);
			printString("Go straight\n\r");
			drive(FORWARD, 120, 120);
			thisTurn = FORWARD;
		}
		else if(leftDelta < 4)
		{
			emergencyBrake(leftInfraReading, rightInfraReading);
			printString("Go left\n\r");
			drive(PIVOT_RIGHT,180,180);
			thisTurn = PIVOT_RIGHT;
		}
		else if(rightDelta < 4)
		{
			emergencyBrake(leftInfraReading, rightInfraReading);
			printString("Go right\n\r");
			drive(PIVOT_LEFT,200,200);
			thisTurn = PIVOT_LEFT;
		}
		else
		{
			emergencyBrake(leftInfraReading, rightInfraReading);
			drive(BACKWARD, 140, 100);
			_delay_ms(50);
			printString("Brake-Backward-Pivot\n\r");
			thisTurn = BACKWARD;
		}
		
		//Corner escape --------------------------------------------
		if((thisTurn != lastTurn) || (thisTurn==BACKWARD))
		{
			if((cornerCount ==4 ) && (thisTurn == BACKWARD))
			{
				drive(PIVOT_RIGHT, 200,200);
				cornerCount = 0;
			}	
			else if(cornerCount ==4)
			{
				drive(lastTurn, 200, 200);
				cornerCount = 0;
			}
			else
			{
				cornerCount++;
			}
		}
		else
		{
			cornerCount = 0;
		}
		lastTurn = thisTurn;
		_delay_ms(100);
		// ---------------------------------------------------------
		
		//Debugging ------------------------------------------------
		debugPrint("leftDelta: ", leftDelta);
		debugPrint("rightDelta: ", rightDelta);
		debugPrint("cornerCount: ", cornerCount);
	}
	return 0;
}

// ----- Functions --------------------------------------------------
// ------------------------------------------------------------------
// ------------------------------------------------------------------

void verifyInfra(void)
{
	/*
	Step One of the Setup Routine
	Set servo arm down to read table top
	Ensure infra readings from table are within range
	*/ 
	//Local variables ----------------------------------------------
	uint8_t leftDelta=0;
	uint8_t rightDelta=0;
	uint16_t leftInfraReading=0;
	uint16_t rightInfraReading=0;
	
	// Place servo in down position --------------------------------
	DDRB |= (1 << PB1);
	OCR1A = pgm_read_word(pulseLenArr + POS_INDEX_1); 
	_delay_ms(40); 									  
	DDRB &= ~(1 << PB1); 							  
	
	// Event Loop --------------------------------------------------
	for(uint8_t pos_one_cnt=0;pos_one_cnt<20; pos_one_cnt = pos_one_cnt)
	{
		//Sonic Sensor Reading---------------------------
		leftDelta = sensorDelta(ultraSonic1(), pgm_read_word(lSonicLookUp+POS_INDEX_1));
		rightDelta = sensorDelta(ultraSonic2(), pgm_read_word(rSonicLookUp+POS_INDEX_1));;		
		
		leftInfraReading = getReading(LEFT_INFRA_CH);
		rightInfraReading = getReading(RIGHT_INFRA_CH);

		if((leftDelta < 4) && (rightDelta <4))
		{
			if((leftInfraReading < 950) & (rightInfraReading < 950))
			{
				pos_one_cnt++;
			}
		}
		else
		{
			pos_one_cnt = 0;
		}
		_delay_ms(20);
	}
}

void emergencyBrake(uint16_t leftReading, uint16_t rightReading)
{
	if(leftReading > 950 || rightReading > 950)
	{
		//drive(BRAKE, 0, 0);
		printString("Brake");
	}
}

uint8_t sensorDelta(uint16_t reading, uint16_t threshold)
{
	/* 
	Takes a sensor reading and an expected sensor threshold as inputs
	Returns the absolute value of the difference
	*/
	uint16_t delta = 0;
	
	if(reading > threshold)
	{
		delta = reading - threshold;
	}
	else if(threshold > reading)
	{
		delta = threshold - reading;
	}
	return delta;
}

void toggleLed(void)
{
	/* 
	Toggles an LED for indication warning
	*/
	uint8_t i = 0;	
	for (i=0; i < 8; i++)
	{
		PORTB ^= (1 << PB5);
		_delay_ms(100);
	}
	PORTB |= (1 << PB5);
}