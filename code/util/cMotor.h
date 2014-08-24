#include <avr/io.h>

/*------------------------------------------------------------------
Notes
-MotorA is the driver side motor 
-MotorB is the passenger side motor
------------------------------------------------------------------*/

/*------------------------------------------------------------------
Pin, PORT, and DDR definitions
------------------------------------------------------------------*/

#define 	MOTOR_A_EN			PD6
#define		MOTOR_A_EN_DDR		DDRD
#define		MOTOR_A_EN_PORT		PORTD

#define 	MOTOR_A_1			PD7
#define 	MOTOR_A_1_PORT		PORTD
#define		MOTOR_A_1_DDR		DDRD

#define 	MOTOR_A_2			PB0
#define		MOTOR_A_2_PORT		PORTB
#define		MOTOR_A_2_DDR		DDRB

#define 	MOTOR_B_EN 			PD5
#define 	MOTOR_B_EN_PORT		PORTD
#define 	MOTOR_B_EN_DDR		DDRD

#define 	MOTOR_B_1			PD4
#define 	MOTOR_B_1_PORT		PORTD
#define		MOTOR_B_1_DDR		DDRD

#define 	MOTOR_B_2			PD3
#define 	MOTOR_B_2_PORT		PORTD
#define		MOTOR_B_2_DDR		DDRD

#define		BRAKE				0
#define		FORWARD				1
#define		BACKWARD			2
#define		COAST				3
#define		PIVOT_RIGHT			4
#define		PIVOT_LEFT			5
#define 	LEFT				6
#define		RIGHT				7
//------------------------------------------------------------------

/*------------------------------------------------------------------
Motor Functions
------------------------------------------------------------------*/
//Initialize Motor A and Motor B Using Timer 0
void initMotorAB(void);

//Set OCR0A To Control PWM Enable of SN754410
void setSpeedA(uint8_t speed);
//Set OCR0B To Control PWM Enable of SN754410
void setSpeedB(uint8_t speed);


void directionA(uint8_t dir);
void directionB(uint8_t dir);
void drive(uint8_t command, uint8_t speedA, uint8_t speedB);
//------------------------------------------------------------------