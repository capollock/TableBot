#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "USART.h"

volatile int counter = 0;

void initTimer0(void)
{
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

// unsigned int ultraSonicSense(void)
// {
	// unsigned int distance = 0;
	// PORTB |= (1 << PB3);
	// _delay_us(40);
	// PORTB &= ~(1 << PB3);
	// loop_until_bit_is_set(PINB, PB4);
	// while(bit_is_set(PINB, PB4))
	// {
		// distance += 1;
		// _delay_us(1);
	// }
	// printString("bit is clear");
	// distance /= 58;
	// return distance;
// }

int ultraSonicSense(void)
{
	counter = 0;
	int distance = 0;
	PORTB |= (1 << PB3);
	_delay_us(10);
	PORTB &= ~(1 << PB3);
	loop_until_bit_is_set(PINB,PB4);
	TCNT0 = 0;
	loop_until_bit_is_clear(PINB, PB4);
	uint8_t ticks = TCNT0-1;
	distance = ((ticks + (counter * 100)) / (29 * 2));
	return distance;
}

void uart_putint(unsigned int i)
{
        unsigned char h,t,o;
        o= (i%10) | 0x30;//ones
        i/=10;
        t= (i%10) | 0x30;//tens
        i=i%100;
        h=(i/10) | 0x30;//hundreds
        transmitByte(h);
        transmitByte(t);
        transmitByte(o);
}


// void uart_putfloat(float f)
// {
        // unsigned int v,p;
        // long int num;
        // num=f*1000;
        // p=num%1000;
        // num=num/1000;
        // v=num;

        // uart_putint(v);
        // transmitByte('.');
        // uart_putint(p);
// }


int main(void)
{
	initTimer0();
	initUSART();
	DDRB |= (1 << PB3);
	DDRB &= ~(1 << PB4);
	int reading=0;
	while(1)
	{
		reading = ultraSonicSense();
		_delay_ms(500);
		printString("Distance");
		uart_putint(reading);
		printString("\r\n");
	}
	return 0;
}