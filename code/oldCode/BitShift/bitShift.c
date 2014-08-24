#include <avr/io.h>
#include <util/delay.h>

#define LED_PORT 		PORTB
#define LED_DDR			DDRB
#define DELAY_TIME		85

void go_right(void)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		LED_PORT |= (1 << i);  // Turn the Switch On
		_delay_ms(DELAY_TIME);
	}
}

void go_left(void)
{
	uint8_t i;
	for (i = 7; i < 255; i--)
	{
		LED_PORT &= ~(1 << i);
		_delay_ms(DELAY_TIME);
	}
}

void toggle_left(void)
{
	uint8_t i;
	for (i=7; i < 255; i--)
	{
		LED_PORT ^= (1 << i);
		if (i != 7)
		{
			LED_PORT ^= (1 << (i+1));
		}
		_delay_ms(DELAY_TIME);
	}
	LED_PORT &= (1 << 0);
}

void toggle_right(void)
{
	uint8_t i;
	for (i=0; i < 8; i++)
	{
		LED_PORT ^= (1 << i);
		if (i != 0)
		{
			LED_PORT ^= (1 << (i-1));
		}
		_delay_ms(DELAY_TIME);
	}
	LED_PORT &= (1 << 7);
}

int main(void)
{
	LED_DDR = 0xff;
	
	while(1)
	{
		 go_right();
		 go_left();
		 go_right();
		 toggle_left();
		 toggle_right();
		 PORTD = 0;
		 _delay_ms(5 * DELAY_TIME);
		 PORTD = 0xff;
		 _delay_ms(5 * DELAY_TIME);
	}
	return (0);
}