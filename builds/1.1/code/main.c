#define F_CPU 14745600L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "serial.c"
#include "counter_74LV8154.c"
#include "timer1hz.c"

void LEDblink()
{
	DDRB |= (1 << PB1); // status LED output
	PORTB |= (1 << PB1);
	_delay_ms(50);
	PORTB &= ~(1 << PB1);
	_delay_ms(50);
}

void SendStartupMessage()
{

	SerialSendBreak();
	SerialSendBreak();
	SerialSendString("USB Counter - www.SWHarden.com");
	SerialSendBreak();
}

int main(void)
{
	SerialInitialize();
	TimerInitialize1Hz();

	SendStartupMessage();
	uint32_t countLast;
	uint32_t countNow;
	uint32_t countDiff;
	for (;;)
	{

		if (gateCycled)
		{
			PORTB |= (1 << PB1);

			countNow = ReadCount();
			countDiff = countNow - countLast;

			SerialSend_intThreeDigit(seconds);
			SerialSendPeriod();
			SerialSend_intThreeDigit(milliseconds);
			SerialSendComma();
			//SerialSend_UINT32(countNow);
			SerialSend_UINT32(countDiff);
			SerialSendBreak();

			countLast = countNow;
			gateCycled = 0;
			
			PORTB &= ~(1 << PB1);
		}
	}
}
