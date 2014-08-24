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
#define LOCK_DIST_CONST 3
//Global -----------------------------------------------------------
uint16_t llSonic=0;
uint16_t lrSonic=0;
uint8_t count=0;
uint8_t targetLock=1;
uint16_t lockDist=2;

//Function Prototypes ----------------------------------------------
void scanTarget(uint8_t leftOrRight);


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
     //If target is not locked = 1
     if(targetLock)
     { 
       //If either sensor in range
       if(llSonic < TOP_RANGE || lrSonic < TOP_RANGE)
       {
         if(llSonic > lrSonic)
         {
           printString("Left loop\n\r");
           scanTarget(1);
         }    
         else if (lrSonic > llSonic)
         { 
           printString("Right loop\n\r");
           scanTarget(0);
         }
         else
         {
           printString("Good enough\n\r");
           drive(BRAKE, 0, 0);
         }
       }
       //Else sensor is out of range
       else
       {
         printString("Out of range\n\r");
         drive(BRAKE, 0, 0);
       }
       //Other items if target not locked
       _delay_ms(20);
       drive(BRAKE, 0 , 0);
     }
     //If target is locked = 0
     else 
     {
       //If either sonic sensor is greater than the target distance
       //Unlock target and scan next loop
       if(llSonic < lockDist || lrSonic < lockDist)
       {
          targetLock=0;
           printString("Drive forward\n\r");

       }
       else
       {
         targetLock=1;
         printString("Unlock\n\r");
       }
     }
 }
  return 0;
}

void scanTarget(uint8_t leftOrRight)  {
  // Declare Variables
  uint16_t lSonic;
  uint16_t targetDist;
  int (*ultraSon)(void);
  uint8_t dir1;
  uint8_t dir2;
  
  
  // Set Variables
  if(leftOrRight) //left =1
  {
    lSonic = llSonic;
    targetDist = lrSonic;
    ultraSon = &ultraSonic3;
    dir1 = PIVOT_RIGHT;
    dir2 = PIVOT_LEFT;
  }
  else
  {
    lSonic = lrSonic;
    targetDist = llSonic;
    ultraSon = &ultraSonic4;
    dir1 = PIVOT_LEFT;
    dir2 = PIVOT_RIGHT;
  }
  lockDist = targetDist + LOCK_DIST_CONST;
  while(lSonic > targetDist && count  < TOP_COUNT)
  {
    lSonic = ultraSon();
    debugPrint("lSonic: ", lSonic);
    staticPrint("targetDist: :", targetDist, "count: ", count);
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
       _delay_ms(10);
     }
     targetLock=0;
   }
}
