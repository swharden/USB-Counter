#define F_CPU 14745600L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "serial.c"
#include "counter-74LV8154.c"
#include "timer-for-timing.c"

int main(void)
{
	SerialInitialize();
	TimerInitialize();

	SendStartupMessage();

	uint32_t countLast;
	uint32_t countNow;
	uint32_t countDiff;

	for (;;)
	{
		if (gateCycled)
		{
			countNow = ReadCount();
			countDiff = countNow - countLast;

			SerialSend_intThreeDigit(seconds);
			SerialSendPeriod();
			SerialSend_intThreeDigit(milliseconds);
			SerialSendComma();
			SerialSend_UINT32(countNow);
			SerialSendComma();
			SerialSend_UINT32(countDiff);
    		SerialSend(10); // new line

			countLast = countNow;
			gateCycled = 0;
		}
	}
}
