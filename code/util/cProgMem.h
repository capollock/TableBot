/*
Flash Memory Storage
Intended Chip: atmega328p
Chip Resources: N/A
Chip Configuration: N/A
External Sensor: N/A
*/

//------------------------------------------------------------------
//----- Includes ---------------------------------------------------
#include <avr/pgmspace.h>
//------------------------------------------------------------------

//------------------------------------------------------------------
//----- Defines ----------------------------------------------------

#define		POS_INDEX_1		0
#define 	POS_INDEX_2		1
#define		POS_INDEX_3		3
//------------------------------------------------------------------

// ------------------------------------------------------------------
// ----- Look Up Table ----------------------------------------------
/*
	This lookup table provides the expected distance to the ground
	based on the angle of the servo arm
	lSonicLookUp - expected reading from left ultraSonic
	rSonicLookUp - expected reading from right ultraSonic
*/
const uint16_t lSonicLookUp[] PROGMEM = {9,28,14,0,0};

const uint16_t rSonicLookUp[] PROGMEM = {9,28,12,12,13};

const uint16_t pulseLenArr[] PROGMEM = {500, 850, 1050, 1100, 1200};
//------------------------------------------------------------------
