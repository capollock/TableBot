/*
Servo Library by Chris Pollock
Based on Make, AVR Programming: Learning to Write Software for Hardware, Elliot Williams

Intended Chip: atmega328p
Chip Resources: Timer1 (16 Bit)
Chip Configuration: Default Clock Speed at 1Mghz 
*/
#include <avr/io.h>

//------------------------------------------------------------------
//----- User Specific Values ---------------------------------------

#define SERVO_MIN 1000
#define SERVO_MAX 2000

//------------------------------------------------------------------


//Initiate Servo Connected to PB1
void initServoOC1A(void);

//Convert uint8_t Angle Argument to Servo Duty Cycle
void angleToDutyCycle(uint8_t angle);



