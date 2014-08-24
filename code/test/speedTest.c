#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include "cMotor.h"
#include "cSonic.h"


//Defines ----------------------------------------------------------
#define BASESPEED 140
#define PIVOT_SPEED 150
#define PIVOT_DELAY 40
#define TOP_RANGE 60
#define TOP_COUNT 10
#define TARGET_LOCK_CONST
//Global -----------------------------------------------------------
uint16_t llSonic=0;
uint16_t lrSonic=0;
uint8_t count=0;
uint8_t targetLock=1;

int main(void)
{
  initMotorAB();
  initSonicTimer1();
  initUSART();
  while(1)
  {
      count = 0;
      llSonic=ultraSonic3();
      _delay_ms(2);
      lrSonic=ultraSonic4();
      printString("Top loop\n\r");
     //If either sensor in range
  if(targetLock)
  { 
      if(llSonic < TOP_RANGE || lrSonic < TOP_RANGE)
      {
         if(llSonic > lrSonic)
         {
           printString("Left loop\n\r");
	   while(llSonic > lrSonic && count  < TOP_COUNT)
           {
              llSonic = ultraSonic3();
              debugPrint("llSonic: ", llSonic);
              staticPrint("lrSonic: :", lrSonic, "count: ", count);
              drive(PIVOT_RIGHT, PIVOT_SPEED, PIVOT_SPEED);
              _delay_ms(1);
              count++;
           }
          //If it didn't time out then go back half of count
           //And set targetLock to True
	   if(count < TOP_COUNT)
           {  
	      for(uint8_t i = count; i > 0; i--)
              {
                 drive(PIVOT_LEFT, PIVOT_SPEED, PIVOT_SPEED);
		 targetLock=0;
                 _delay_ms(1);
              }
           }

         }    
         else if (lrSonic > llSonic)
         { 
           printString("Right loop\n\r");
           while(lrSonic > llSonic && count < TOP_COUNT)
           {
              lrSonic = ultraSonic4();
              debugPrint("lrSonic: ", lrSonic);
              staticPrint("llSonic: :", llSonic, "count: ", count);
              drive(PIVOT_LEFT, PIVOT_SPEED, PIVOT_SPEED);
              _delay_ms(1);
              count++;
           }
          
           //If it didn't time out then go back half of count
           //And set targetLock to True
	   if(count < TOP_COUNT)
           {  
	      for(uint8_t i = count; i > 0; i--)
              {
                 drive(PIVOT_RIGHT, PIVOT_SPEED, PIVOT_SPEED);
		 targetLock=0;
                 _delay_ms(1);
              }
           }
	 }
	 else
         {
            printString("Good enough\n\r");
            drive(BRAKE, 0, 0);
         }
      }
      else
      {
	printString("Out of range\n\r");
        drive(BRAKE, 0, 0);
      }
      _delay_ms(20);
      drive(BRAKE, 0 , 0);
  } 
 }
  return 0;
}



