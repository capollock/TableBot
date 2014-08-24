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
void debugPrint(const char myString[], uint8_t reading);
void emergencyBrake(uint16_t leftReading, uint16_t rightReading);
void incrementSpeed(uint8_t* ptrSpeed);
void decrementSpeed(uint8_t* ptrSpeed);
uint8_t sensorDelta(uint16_t reading, uint16_t threshold);
// ------------------------------------------------------------------

// ----- Global Variables -------------------------------------------
uint8_t lastTurn=0;
uint8_t previousTurn=0;
uint8_t cornerCatch=0;
uint8_t leftSpeed=80;
uint8_t rightSpeed=80;
// ------------------------------------------------------------------

/*
	HBRC Phase One
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
	uint8_t eventCount = 0;

	// -------------------------------------------------------------
	
	//Initialize Routine -------------------------------------------
	toggleLed();
	verifyInfra();
	// -------------------------------------------------------------
	
	// Place servo arm in position ---------------------------------
	DDRB |= (1 << PB1);
	OCR1A = pgm_read_word(pulseLenArr + POS_INDEX_2);
	//--------------------------------------------------------------

	drive(FORWARD, leftSpeed, rightSpeed);
		
	while(1)
	{
		//lastLoop = previousLoop - increment to 10
		leftInfraReading = getReading(LEFT_INFRA_CH);
		rightInfraReading= getReading(RIGHT_INFRA_CH);
		
		leftDelta = sensorDelta(ultraSonic1(), pgm_read_word(lSonicLookUp+POS_INDEX_2));
		rightDelta = sensorDelta(ultraSonic2(), pgm_read_word(rSonicLookUp+POS_INDEX_2));
		
		debugPrint("left delta: ", leftDelta);
		debugPrint("left sonic: ", ultraSonic1());
		debugPrint("left speed: ", leftSpeed);
		
		debugPrint("right delta: ", rightDelta);
		debugPrint("right sonic: ", ultraSonic2());
		debugPrint("right speed: ", rightSpeed);
		
		if((leftDelta < 10) && (rightDelta < 10))
		{
			emergencyBrake(leftInfraReading, rightInfraReading);
			incrementSpeed(&leftSpeed);
			incrementSpeed(&rightSpeed);
			drive(FORWARD, leftSpeed, rightSpeed);
			printString("go straight");
		}
		else if(leftDelta < 10)
		{
			emergencyBrake(leftInfraReading, rightInfraReading);
			printString("Go right\n\r");
			decrementSpeed(&leftSpeed);
			incrementSpeed(&rightSpeed);
			drive(FORWARD, leftSpeed, rightSpeed);
		}
		else if(rightDelta < 10)
		{
			emergencyBrake(leftInfraReading, rightInfraReading);
			printString("Go left\n\r");
			incrementSpeed(&leftSpeed);
			decrementSpeed(&rightSpeed);
			drive(FORWARD, leftSpeed, rightSpeed);
		}
		else if(eventCount == 100)
		{
			emergencyBrake(leftInfraReading, rightInfraReading);
			drive(BRAKE, 0,0);
			_delay_ms(100);
			drive(BACKWARD, 120, 120);
			_delay_ms(40);
			printString("Brake-Backward-Pivot\n\r");
			eventCount = 0;
		}
		else
		{
			eventCount++;
		}
		_delay_ms(10);
	}
	return 0;
}

// ----- Functions --------------------------------------------------
// ------------------------------------------------------------------
// ------------------------------------------------------------------
inline void incrementSpeed(uint8_t* ptrSpeed)
{
	if(*ptrSpeed != 220)
	{
		*ptrSpeed+=40;
	}
}

inline void decrementSpeed(uint8_t* ptrSpeed)
{
	if(*ptrSpeed != 0)
	{
		*ptrSpeed-=40;
	}
}

void debugPrint(const char myString[], uint8_t reading)
{
	printString(myString);
	printWord(reading);
	printString("\n\r");
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
		_delay_ms(50);
	}
}

void emergencyBrake(uint16_t leftReading, uint16_t rightReading)
{
	if(leftReading > 950 || rightReading > 950)
	{
		drive(BRAKE, leftSpeed, rightSpeed);
		_delay_ms(100);
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