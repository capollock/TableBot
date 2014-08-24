#include <avr/io.h>
#include <util/delay.h>

#define IN_PORT		PORTD
#define IN_PIN 		PIND
#define IN_DDR		DDRD
#define OUT_PORT 	PORTB
#define OUT_DDR		DDRB

int main(void)
{
	//Set PORTD to Input and Set Pull-Up Resistor
	IN_DDR 	= 0x00;
	IN_PORT = 0xff;
	
	//Set PORTB to Output and Initialize to LOW
	OUT_DDR = 0xff;
	
	uint8_t i;
	OUT_PORT = 0b00000011;
	_delay_ms(200);
	OUT_PORT = 0b00000000;
	
	while(1)
	{
		//READ IN_PIN for Button Push on PD2
		if (IN_PIN == 0b11111101)
		{
			i+=1;
			if (i == 1)
			{
				OUT_PORT |= (1 << 0);
			}
			else if (i == 2)
			{
				OUT_PORT |= (1 <<1);
			}
			else if (i ==3)
			{
				i = 0;
				OUT_PORT = 0;
			}
		}
		

		_delay_ms(100);
	}
	return (0);
}