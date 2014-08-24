#include <avr/io.h>
#include <util/delay.h>
#include "USART.h"


// #define TRIG_1 			PC5
// #define TRIG_1_DDR		DDRC
// #define TRIG_1_PIN		PINC
// #define	TRIG_1_PORT		PORTC

// #define ECHO_1			PC4
// #define ECHO_1_DDR		DDRC
// #define ECHO_1_PIN		PINC
// #define ECHO_1_PORT		PORTC

// unsigned long clockTicks(void)
// {
	// unsigned long ticks=0;
	// //Data Direction Registers: Trig_1 Out Echo_1 in
	// TRIG_1_DDR |= (1 << TRIG_1);
	// ECHO_1_DDR &= ~(1 << ECHO_1);
	
	// TRIG_1_PORT |= (1 << TRIG_1);
	// _delay_us(10);
	// TRIG_1_PORT &= ~(1 << TRIG_1);
	// loop_until_bit_is_set(ECHO_1_PIN, ECHO_1);
	// while(bit_is_set(ECHO_1_PIN, ECHO_1)
	// {
		// ticks +=1
	// }
	// return ticks;
// }

void printLong(unsigned long aLong) 
{
  transmitByte('0' + (aLong / 1000000000));				/*Billion*/
  transmitByte('0' + ((aLong / 100000000)% 10));		/*Hundred-million*/
  transmitByte('0' + ((aLong / 10000000)% 10));			/*Ten-million*/
  transmitByte('0' + ((aLong / 1000000)% 10));			/* Millions*/
  transmitByte('0' + ((aLong / 100000)% 10));			/* Hundred-thousands*/
  transmitByte('0' + ((aLong / 10000)% 10));             /* Ten-thousands */
  transmitByte('0' + ((aLong / 1000) % 10));        	/* Thousands */
  transmitByte('0' + ((aLong/ 100) % 10));           	/* Hundreds */
  transmitByte('0' + ((aLong/ 10) % 10));            	/* Tens */
  transmitByte('0' + (aLong % 10));                  	/* Ones */
}

int main(void)
{
	initUSART();
	DDRB |= (1 << PB5);
	PORTB |= (1 << PB5);
	unsigned long mTicks = 0;
	
	while(1)
	{
		mTicks = 1;
		printLong(mTicks);
		printString("\r\n");
		_delay_ms(5000);
		mTicks = 12;
		printLong(mTicks);
		printString("\r\n");
		_delay_ms(5000);
		mTicks = 123;
		printLong(mTicks);
		printString("\r\n");
		_delay_ms(5000);
		mTicks = 1234;
		printLong(mTicks);
		printString("\r\n");
		_delay_ms(5000);
		mTicks = 12345;
		printLong(mTicks);
		printString("\r\n");
		_delay_ms(5000);
		mTicks = 123456;
		printLong(mTicks);
		printString("\r\n");
		_delay_ms(5000);
		mTicks = 1234567;
		printLong(mTicks);
		printString("\r\n");
		_delay_ms(5000);
		mTicks = 12345678;
		printLong(mTicks);
		printString("\r\n");
		_delay_ms(5000);
		mTicks = 123456789;
		printLong(mTicks);
		printString("\r\n");
		_delay_ms(5000);
		mTicks = 1234567890;
		printLong(mTicks);
		printString("\r\n");
		_delay_ms(5000);
	}
	
}

