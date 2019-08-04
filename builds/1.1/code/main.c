#define F_CPU 14745600L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "serial.c"
#include "counter-74LV8154.c"
#include "display-max7219-7seg.c"
#include "timer-for-timing.c"

int main(void)
{

	// WARNING: code hangs if no USB port is connected.

	SpiInitialize();

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

			DisplayInitialize();
			DisplayNumber(countDiff);
		}
	}
}
