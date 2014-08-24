/*
Ultrasonic Library by Chris Pollock
Intended Chip: atmega328p
Chip Resources: Timer Counter1 (16 Bit)
Chip Configuration: Default Clock Speed at 1MGhz 
External Sensor: HC-SR04 UltraSonic Rangefinder
*/

#include <avr/io.h>

/*------------------------------------------------------------------
User Definitions
------------------------------------------------------------------*/

//Upper Driver Side
#define TRIG_DDR1 DDRB 
#define TRIG_PIN1 PB4 		//Digital Pin 12
#define TRIG_PORT1 PORTB

#define ECHO_DDR1 DDRC
#define ECHO_PORT1 PORTC
#define ECHO_PIN1 PC5		//Analog Input 5
#define ECHO_IN1 PINC

//Upper Passenger Side
#define TRIG_DDR2 DDRB
#define TRIG_PIN2 PB5		//Digital Pin 13
#define TRIG_PORT2 PORTB

#define ECHO_DDR2 DDRC
#define ECHO_PORT2 PORTC
#define ECHO_PIN2 PC2		//Analog Input 2
#define ECHO_IN2 PINC


// Lower Driver Side
#define TRIG_DDR3 DDRB 
#define TRIG_PIN3 PB3		//Digital Pin 11
#define TRIG_PORT3 PORTB

#define ECHO_DDR3 DDRB
#define ECHO_PORT3 PORTB
#define ECHO_PIN3 PB2		//Digital Pin 10
#define ECHO_IN3 PINB

//Lower Passenger Side
#define TRIG_DDR4 DDRC
#define TRIG_PIN4 PC4		//Analog Input 4
#define TRIG_PORT4 PORTC

#define ECHO_DDR4 DDRC
#define ECHO_PORT4 PORTC
#define ECHO_PIN4 PC3		//Analog Input 3
#define ECHO_IN4 PINC





//------------------------------------------------------------------

//Initiate Timer1 Configuration for Counting
void initSonicTimer1(void);

//Get Reading from Ultrasonic Sensor

// Upper Driver Side
int ultraSonic1(void);
// Upper Passenger Side
int ultraSonic2(void);
// Lower Driver Side
int ultraSonic3(void);
//Lower Passenger Side
int ultraSonic4(void);
/*
trigPIN is the PxN of the trigger pin
trigPORT is the PORTx
echoPIN is the PxN of the echo pin
echoIN is the PINx of the echo pin
*/
