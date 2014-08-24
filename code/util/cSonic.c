#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "cSonic.h"
#include "USART.h"

void initSonicTimer1(void)
{
  /* 
  Set Data Direction Registers
  */
  TRIG_DDR1 |= (1 << TRIG_PIN1);
  TRIG_DDR2 |= (1 << TRIG_PIN2);

  ECHO_DDR1 &= ~(1 << ECHO_PIN1);
  ECHO_DDR2 &= ~(1 << ECHO_PIN2); 
 
  TRIG_DDR3 |= (1 << TRIG_PIN3);
  TRIG_DDR4 |= (1 << TRIG_PIN4);

  ECHO_DDR3 &= ~(1 << ECHO_PIN3);
  ECHO_DDR4 &= ~(1 << ECHO_PIN4); 

  /* 
  Initate Timer
  */
  //Normal Mode
  //All bits clear in TCCR1A
  //Prescale by 1 - Assumes Clock Speed Runs at 1 Mghz
  TCCR1B |= (1 << CS10);
  
}


int ultraSonic1(void)
{
	int distance = 0;
	uint16_t time =0;
	TRIG_PORT1 |= (1 << TRIG_PIN1);
	_delay_us(10);
	TRIG_PORT1 &= ~(1 << TRIG_PIN1);
	loop_until_bit_is_set(ECHO_IN1,ECHO_PIN1);
	TCNT1 = 0;
	loop_until_bit_is_clear(ECHO_IN1, ECHO_PIN1);
	time = TCNT1;
	distance = time  / 58;
	return distance;
}

int ultraSonic2(void)
{
	int distance = 0;
	uint16_t time =0;
	TRIG_PORT2 |= (1 << TRIG_PIN2);
	_delay_us(10);
	TRIG_PORT2 &= ~(1 << TRIG_PIN2);
	loop_until_bit_is_set(ECHO_IN2,ECHO_PIN2);
	TCNT1 = 0;
	loop_until_bit_is_clear(ECHO_IN2, ECHO_PIN2);
	time = TCNT1;
	distance = time / 58;
	return distance;
}

int ultraSonic3(void)
{
	int distance = 0;
	uint16_t time =0;
	TRIG_PORT3 |= (1 << TRIG_PIN3);
	_delay_us(10);
	TRIG_PORT3 &= ~(1 << TRIG_PIN3);
	loop_until_bit_is_set(ECHO_IN3,ECHO_PIN3);
	TCNT1 = 0;
	loop_until_bit_is_clear(ECHO_IN3, ECHO_PIN3);
	time = TCNT1;
	distance = time / 58;
	return distance;
}
int ultraSonic4(void)
{
	int distance = 0;
	uint16_t time =0;
	TRIG_PORT4 |= (1 << TRIG_PIN4);
	_delay_us(10);
	TRIG_PORT4 &= ~(1 << TRIG_PIN4);
	loop_until_bit_is_set(ECHO_IN4,ECHO_PIN4);
	TCNT1 = 0;
	loop_until_bit_is_clear(ECHO_IN4, ECHO_PIN4);
	time = TCNT1;
	distance = time / 58;
	return distance;
}
