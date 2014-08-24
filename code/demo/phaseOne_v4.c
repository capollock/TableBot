#include <avr/io.h>
#include <util/delay.h>
#include "USART.h" 			// Serial Comm for Debugging
#include "cSonic.h"			// Library for Ultra Sonic 
#include "cServo.h"                     // Library for Servo
#include "cInfra.h"			// Library for Infrared 
#include "cMotor.h"			// Library for Motor

// ----- DEFINES -------------------------------------------
#define SERVO_POS_ONE 	1500
#define SERVO_POS_TWO 	950
#define L_EXP_DIST	9
#define R_EXP_DIST	8
// ----- Function Prototypes ---------------------------------------
void emergencyBrake(void);
uint8_t sensorDelta(uint16_t reading, uint16_t threshold);
void servoArm(uint16_t timeVal);
// ------------------------------------------------------------------

// ----- Global Variables -------------------------------------------
//Infrared readings
uint16_t leftInfraReading=0;
uint16_t rightInfraReading=0;
	
//Sonic readings
uint16_t ulSonic=0;
uint16_t urSonic=0;
uint16_t llSonic=0;
uint16_t lrSonic=0;

uint16_t leftDelta=0;
uint16_t rightDelta=0;

//Loop Control Variables
uint8_t thisTurn=0;
uint8_t lastTurn=0;
uint8_t cornerCount=0;

// ------------------------------------------------------------------

/*
*****HBRC Phase One*****
Challenge: Drive to One Side of the Table and Go Back to the Other
Approach: Four Corners Approach (i.e., keep turning right once sense the edge)
*/

int main(void)
{
	
  //Bit Setting --------------------------------------------------
  servoArm(SERVO_POS_ONE);
  _delay_ms(1000);
  initUSART();
  initSonicTimer1();
  initInfraADC();
  initMotorAB();
  servoArm(SERVO_POS_TWO);
// -------------------------------------------------------------

  // Local variables ---------------------------------------------
  //None
  // -------------------------------------------------------------
	
  //Initialize Routine -------------------------------------------
  //verifyInfra();
  // -------------------------------------------------------------
	
  while(1)
  {
      // Get sensor data-----------------------------------------
      leftInfraReading = getReading(LEFT_INFRA_CH);
      rightInfraReading= getReading(RIGHT_INFRA_CH);	
      ulSonic=ultraSonic1();
      urSonic=ultraSonic2();
      llSonic=ultraSonic3();
      lrSonic=ultraSonic4();
      
      //Emergency Check
      emergencyBrake();
	
      //Check Delta of Actual vs. Expected Foor Upper Sonics
      leftDelta = sensorDelta(ultraSonic1(), L_EXP_DIST);
      rightDelta = sensorDelta(ultraSonic2(), R_EXP_DIST);
		
		
      // Main conditionals  --------------------------------------
      if((leftDelta < 4) && (rightDelta < 4))
      {
        emergencyBrake();
	printString("Go straight\n\r");
	drive(FORWARD, 120, 120);
	thisTurn = FORWARD;
      }
      else if(leftDelta < 4)
      {
	emergencyBrake();
	printString("Go left\n\r");
	drive(PIVOT_LEFT,180,180);
	thisTurn = PIVOT_LEFT;
      }
      else if(rightDelta < 4)
      {
        emergencyBrake();
	printString("Go right\n\r");
	drive(PIVOT_RIGHT,200,200);
	thisTurn = PIVOT_RIGHT;
      }
      else
      {
	emergencyBrake();
	drive(BACKWARD, 140, 100);
	_delay_ms(50);
	printString("Brake-Backward-Pivot\n\r");
	thisTurn = BACKWARD;
      }
		
      //Corner escape --------------------------------------------
      if((thisTurn != lastTurn) || (thisTurn==BACKWARD))
      {
	if((cornerCount ==2) && (thisTurn == BACKWARD))
	{
	  drive(PIVOT_RIGHT, 200,200);
	  cornerCount = 0;
	  _delay_ms(300);	
	}	
	else if(cornerCount ==4)
	{
	  drive(PIVOT_RIGHT, 200, 200);
	  cornerCount = 0;
          _delay_ms(300);
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
   } 
   //--------------------------------------------------  
  return 0;
}

// ----- Functions --------------------------------------------------
// ------------------------------------------------------------------
// ------------------------------------------------------------------

void emergencyBrake(void)
{  

 if(leftInfraReading > 950 || rightInfraReading > 950)
 { 
       drive(BACKWARD, 150, 150);
       _delay_ms(300);
       if (leftInfraReading > rightInfraReading)
       {
           drive(PIVOT_LEFT, 230, 230);

           _delay_ms(800);
       }
       else
       { 
	  drive(PIVOT_RIGHT, 230, 230); 
          _delay_ms(800);
       }
      drive(BRAKE, 0, 0);
      _delay_ms(20);
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

void servoArm(uint16_t timeVal)
{
  //Initialize Timer1 for Servo
  initServoOC1A();
  _delay_ms(20);
  //Set PB1 to Output
  DDRB |= (1 << PB1);
  //Set Duty Cycle
   OCR1A = timeVal;
  //Wait Until The Servo Arm Is In Place
  _delay_ms(2000);
  //Set PB1 to Input
 DDRB &= ~(1 << PB1);
  //Re Initialize Timer for UltraSonic Sensors
  initSonicTimer1();
}
