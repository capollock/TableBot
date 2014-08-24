#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"
#include "cMotor.h"
#include "cSonic.h"


//Defines ----------------------------------------------------------
#define BASESPEED 140
#define PIVOT_SPEED 175
#define PIVOT_DELAY 40
#define TOP_RANGE 60
#define TOP_COUNT 20
//Global -----------------------------------------------------------
uint16_t llSonic=0;
uint16_t lrSonic=0;
uint8_t count=0;
uint8_t targetLock=1;
uint16_t leftLock=0;
uint16_t rightLock=0;

//Function Prototypes ----------------------------------------------
void scanTarget(uint16_t* lSonic, uint16_t targetDist, int (*ultraSon)(void), uint8_t dir1, uint8_t dir2);


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
           scanTarget(&llSonic, lrSonic, ultraSonic3, PIVOT_RIGHT, PIVOT_LEFT);
         }    
         else if (lrSonic > llSonic)
         { 
           printString("Right loop\n\r");
           scanTarget(&lrSonic, llSonic, ultraSonic4, PIVOT_LEFT, PIVOT_RIGHT);
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

//Pointer to the low variable, pointer to the high variable, pointer to the ultraSonic func, pivot direction
void scanTarget(uint16_t* lSonic, uint16_t* tSonic, int (*ultraSon)(void), uint8_t dir1, uint8_t dir2)
{
  while(*lSonic > targetDist && count  < TOP_COUNT)
  {
    *lSonic = ultraSon();
    debugPrint("lSonic: ", *lSonic);
    staticPrint("targetDist: :", *targetDist, "count: ", count);
    drive(dir1, PIVOT_SPEED, PIVOT_SPEED);
    _delay_ms(1);
    count++;
   }
   //If it didn't time out then go back half of count
   //And set targetLock to True
   if(count < TOP_COUNT)
   {  
     for(uint8_t i = count; i > 0; i--)
     {
       drive(dir2, PIVOT_SPEED, PIVOT_SPEED);
       targetLock=0;
       targetOne=
       
       _delay_ms(10);
     }
   }
}