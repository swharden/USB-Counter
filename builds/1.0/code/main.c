#define F_CPU 10000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "serial.c"

void LEDblink()
{
	PORTB |= (1 << PB1);
	_delay_ms(50);
	PORTB &= ~(1 << PB1);
	_delay_ms(50);
}

void InitializeIO()
{
	DDRB |= (1 << PB1); // status LED
	DDRB |= (1 << PB2); // counter gate
	DDRC |= 0b00001111; // counter A0-A3
	DDRD = 0;			// all inputs
	PORTD = 255;
}

char SetRegister(char registerNumber)
{
	switch (registerNumber)
	{
	case (3):
		PORTC = 0b00000111; // A3
		break;
	case (2):
		PORTC = 0b00001011; // A2
		break;
	case (1):
		PORTC = 0b00001101; // A1
		break;
	case (0):
		PORTC = 0b00001110; // A0
		break;
	}
}

uint8_t ReadRegister(char registerNumber)
{
	SetRegister(registerNumber);

	uint8_t value = 0;

	if ((PINC & (1 << PC4)))
		value |= 0b00000001;

	if ((PINC & (1 << PC5)))
		value |= 0b00000010;

	if ((PIND & (1 << PD2)))
		value |= 0b00000100;

	if ((PIND & (1 << PD3)))
		value |= 0b00001000;

	if ((PIND & (1 << PD4)))
		value |= 0b00010000;

	if ((PIND & (1 << PD5)))
		value |= 0b00100000;

	if ((PIND & (1 << PD6)))
		value |= 0b01000000;

	if ((PIND & (1 << PD7)))
		value |= 0b10000000;

	return value;

}

void ReadCount()
{
	uint32_t value = 0;

	value += ReadRegister(3);
	value = value << 8;
	
	value += ReadRegister(2);
	value = value << 8;

	value += ReadRegister(1);
	value = value << 8;

	value += ReadRegister(0);

	SerialSendLongASCII(value);
	
}

void InitializeOneHzTimer()
{
	// use the 16-bit timer to create a 1Hz event

	TCCR1A |= _BV(COM1A1); // Clear OC1A/OC1B on Compare Match when upcounting
	TCCR1B |= _BV(WGM13);  // enable "PWM, phase and frequency correct"

	TCCR1B |= _BV(CS10) | _BV(CS11); // prescale by 64

	ICR1 = 25178;	 // upper limit pulse width (max 2^16-1)
	DDRB |= _BV(PB1); // enable output on PB1 (OC1A)
	OCR1A = 0;		  // set set the initial pulse width

	TIMSK1 |= 1; // overflow interrupt enable

	sei();
}

ISR(TIMER1_OVF)
{
	LEDblink();
}

/*
ISR(TIMER1_OVF_vect)
{
	LEDblink();
}
*/

int main(void)
{

	InitializeIO();
	SerialInitialize();
	//InitializeOneHzTimer();

	SerialSendBreak();
	SerialSendBreak();
	SerialSendString("USB Counter - SWHarden.com");

	for (;;)
	{
		ReadCount();
		SerialSendBreak();
		LEDblink();
		LEDblink();
	}
}
