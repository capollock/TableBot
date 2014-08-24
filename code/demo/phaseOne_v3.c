#include <avr/io.h>
#include <util/delay.h>
#include "USART.h" 			// Serial Comm for Debugging
#include "cServo.h"			// Library for Controlling Servo
#include "cSonic.h"			// Library for Ultra Sonic 
#include "cInfra.h"			// Library for Infrared 
#include "cMotor.h"			// Library for Motor
#include "cProgMem.h"			// Source of Look Up Table in Flash

// ----- DEFINES -------------------------------------------

#define THREE_QUARTER   	0.75
#define ONE_HALF		0.5
#define ONE_QUARTER		0.25

#define FULL_SPEED		200
#define THREE_QUARTER_SPEED 	150
#define ONE_HALF_SPEED 		100
#define QUARTER_SPEED		50

// ------------------------------------------------------------------

// ----- Function Prototypes ---------------------------------------
void toggleLed(void);
void verifyInfra(void);
void emergencyBrake(uint16_t leftReading, uint16_t rightReading);
uint8_t sensorDelta(uint16_t reading, uint16_t threshold);
uint8_t phaseTwoSpeed(uint16_t reading, float expectedDistance); 

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

	//Infrared readings
	uint16_t leftInfraReading=0;
	uint16_t rightInfraReading=0;
	
	//Sonic readings
	uint16_t leftSonicReading=0;
	uint16_t rightSonicReading=0;

	//Speed related variables
	uint8_t leftDelta=0;
	uint8_t rightDelta=0;
	float rightExpectedDistance=0.0;
	float leftExpectedDistance=0.0;
	uint8_t* leftSpeed=0;
	uint8_t* rightSpeed=0;
	
	//Loop control variables
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
	leftExpectedDistance = pgm_read_word(lSonicLookUp+POS_INDEX_2);
	rightExpectedDistance = pgm_read_word(rSonicLookUp+POS_INDEX_2);
		
	while(1)
	{
		// Get sensor data-----------------------------------------
		leftInfraReading = getReading(LEFT_INFRA_CH);
		rightInfraReading= getReading(RIGHT_INFRA_CH);
		
		leftSonicReading=ultraSonic1();
		rightSonicReading=ultraSonic2();
		

		//Testing
		printString("Test 1: \n\r");		
		phaseTwoSpeed(0, 100.0);		
		_delay_ms(1000);		

		printString("Test 2: \n\r");
		phaseTwoSpeed(10, 100.0);
		_delay_ms(1000);		

		printString("Test 3: \n\r");
		phaseTwoSpeed(30, 100.0);
		_delay_ms(1000);		
		
		printString("Test 4: \n\r");
		phaseTwoSpeed(50, 100.0);
		_delay_ms(1000);

		printString("Test 5: \n\r");
		phaseTwoSpeed(51, 100.0);
		_delay_ms(1000);
		
		printString("Test 6: \n\r");
		phaseTwoSpeed(76, 100.0);
		_delay_ms(1000);

		printString("Test 7: \n\r");
		phaseTwoSpeed(100, 100.0);
		_delay_ms(1000);
		// ---------------------------------------------------------
	}
	return 0;
}

// ----- Functions --------------------------------------------------
// ------------------------------------------------------------------
// ------------------------------------------------------------------

uint8_t phaseTwoSpeed(uint16_t reading, float expectedDistance)
{
	//Local variables	
	uint8_t returnSpeed=0;
	float testRatio= reading / expectedDistance;
	
	//Set the returnSpeed based on the conditional
	if (testRatio==0)
	{
		returnSpeed=0;
	}
	else if (testRatio < ONE_QUARTER)
	{
		returnSpeed = QUARTER_SPEED;
	}
	else if (testRatio < ONE_HALF)
	{
		returnSpeed = ONE_HALF_SPEED;
	}
	else if (testRatio < THREE_QUARTER)
	{
		returnSpeed = THREE_QUARTER_SPEED;
	}
	else
	{
		returnSpeed = FULL_SPEED;
	}
	
	//Debugging
	debugPrint("testRatio: ", testRatio);
	debugPrint("returnSpeed: ", returnSpeed);

	//Return uint8_t returnSpeed	
	return returnSpeed;			
}


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
