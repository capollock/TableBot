#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "cSonic.h"

volatile int counter = 0;

void initSonic(void)
{
	/*
	Set Data Direction Registers
	*/
	TRIG_DDR1 |= (1 << TRIG_PIN1);
	TRIG_DDR2 |= (1 << TRIG_PIN2);
	ECHO_DDR1 &= ~(1 << ECHO_PIN1);
	ECHO_DDR2 &= ~(1 << ECHO_PIN2);
	/*
	Initiate Timer
	*/
	//Compare Output Mode: Normal Mode (by Default)
	//Wave Generation Mode: CTC Mode
	TCCR0A |= (1 << WGM01);
	//Clock Select: Enable clock no prescaler
	TCCR0B |= (1 << CS00);
	//Timer Interrput Mask Reg: Enable Interrupt on Compare Match A
	TIMSK0 |= (1 << OCIE0A);
	//Enable Interrupts Globally
	sei();
	//Set Compare Match A to 100 (i.e. every 100 us)
	OCR0A = 100;	
}

ISR(TIMER0_COMPA_vect)
{
	counter += 1;
}

int ultraSonic1(void)
{
	counter = 0;
	int distance = 0;
	TRIG_PORT1 |= (1 << TRIG_PIN1);
	_delay_us(10);
	TRIG_PORT1 &= ~(1 << TRIG_PIN1);
	loop_until_bit_is_set(ECHO_IN1,ECHO_PIN1);
	TCNT0 = 0;
	loop_until_bit_is_clear(ECHO_IN1, ECHO_PIN1);
	uint8_t ticks = TCNT0-2;
	distance = ((ticks + (counter * 100)) / (29 * 2));
	return distance;
}

int ultraSonic2(void)
{
	counter = 0;
	int distance = 0;
	TRIG_PORT2 |= (1 << TRIG_PIN2);
	_delay_us(10);
	TRIG_PORT2 &= ~(1 << TRIG_PIN2);
	loop_until_bit_is_set(ECHO_IN2,ECHO_PIN2);
	TCNT0 = 0;
	loop_until_bit_is_clear(ECHO_IN2, ECHO_PIN2);
	uint8_t ticks = TCNT0-2;
	distance = ((ticks + (counter * 100)) / (29 * 2));
	return distance;
}
