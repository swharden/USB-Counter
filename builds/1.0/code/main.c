#define F_CPU 10000000L
#include <avr/io.h>
#include <util/delay.h>

#include "serial.c"

ledBlinkForever()
{
	DDRB |= (1 << PB1); // set PD6 (pin 11) output
	for (;;)
	{
		PORTB |= (1 << PB1);  // set PD
		_delay_ms(200);		  // wait
		PORTB &= ~(1 << PB1); // set PD6 low
		_delay_ms(800);		  // wait
	}
}

int main(void)
{
	SerialInitialize();
	long count = 0;
	for(;;){
		SerialSendBreak();
		SerialSendLongASCII(count++);
	}
}
