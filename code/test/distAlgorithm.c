#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include "cSonic.h"
#include "cMotor.h"
#include "cServo.h"

#define	SERVO_POS	850
#define SPEED_ONE	50
#define SPEED_TWO	150
#define SPEED_THREE	150
#define SPEED_FOUR	200

void staticPrint(char aString[], uint16_t leftReading, char bString[], uint16_t rightReading);
uint8_t phaseTwoSpeed(uint16_t dist);

int main(void)
{
  initUSART();
  initSonicTimer1();
  initMotorAB(); 
  
  uint16_t leftSonic = 0;
  uint16_t rightSonic = 0;
  uint16_t lowLeftSonic=0;
  uint16_t lowRightSonic=0;

  uint8_t leftSpeed=0;
  uint8_t rightSpeed=0;
  
  setDutyCycle(SERVO_POS);  
  printString("Start");
  while(1)
  {
    leftSonic = ultraSonic1();
     _delay_ms(50);
   
     rightSonic = ultraSonic2();
     _delay_ms(50);

    staticPrint("left sonic: ", leftSonic, "right sonic: ", rightSonic);   
   
    lowLeftSonic = ultraSonic3();
   
     _delay_ms(50); 
    
    lowRightSonic = ultraSonic4();
    _delay_ms(50);
         
    staticPrint("low left: ", lowLeftSonic, "low right: ", lowRightSonic);   
 
    _delay_ms(500);
  }
  return 0;
}

uint8_t phaseTwoSpeed(uint16_t dist)
{

  uint8_t speed=0; 

  if (dist < 10)
  {
      speed = SPEED_ONE;
  }
  else if (dist < 20)
  {
      speed = SPEED_TWO;
  } 
  else if (dist < 30)
  {
      speed = SPEED_THREE;
  }
  else 
  {
      speed = SPEED_FOUR;
  }
  return speed;
}
