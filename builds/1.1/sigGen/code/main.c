#define F_CPU 14745600L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= (1 << PB1); // LED output
	PORTB |= _BV(PB1); // LED on

	for (;;)
	{
		DDRB |= (1 << PB2); // pulse output
		PORTB |= _BV(PB2);
		PORTB &= ~_BV(PB2);
		_delay_us(20);
	}
}
