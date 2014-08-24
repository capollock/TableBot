/*
Infrared Edge Detection by Chris Pollock
Intended Chip: atmega328p
Chip Resources: ADC Channel 0 and Channel 1
Chip Configuration: Default Clock Speed at 1MGhz
External Sensor: RadioShack Infrared LED Emitter and Detector, #276-142
*/

#include <avr/io.h>

//------------------------------------------------------------------
//----- User Specific Values ---------------------------------------

#define LEFT_INFRA_CH	0
#define	RIGHT_INFRA_CH	1
//------------------------------------------------------------------

/*
Initialize ADC Hardware:
-Reference voltage on AVcc
-Default 10 Bit Resolution
-Prescaler Divide by 8
*/
void initInfraADC(void);

/*
Get reading from the ADC Hardware
-Pass uint8_t channel (0-5) as function parameter
-ADC reading contains a 10 bit value (max 1024)
-Return reading in a uint16_t variable
*/
uint16_t getReading(uint8_t mux_channel);