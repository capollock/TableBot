#include <avr/io.h>
#include "cInfra.h"

void initInfraADC()
{
	ADMUX |= (1 << REFS0);                  	//Ref AVcc 
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0);    	//ADC clock prescaler Divide by 8
	ADCSRA |= (1 << ADEN);                      //Enable ADC
}

uint16_t getReading(uint8_t mux_channel)
{
	ADMUX = (0xf0 & ADMUX) | mux_channel;
	ADCSRA |= (1 << ADSC);
	loop_until_bit_is_clear(ADCSRA, ADSC);
	return (ADC);
}