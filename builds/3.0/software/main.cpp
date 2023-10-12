#define F_CPU 24000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "LedManager.h"
#include "NumberSpeaker.h"
#include "SerialManager.h"
#include "ButtonManager.h"

LedManager leds = LedManager();
SerialManager serial = SerialManager();
ButtonManager button = ButtonManager();

volatile uint32_t COUNTER;
ISR(TCD0_OVF_vect)
{
	COUNTER+=4096;
	TCD0.INTFLAGS = TCD_OVF_bm;
}

volatile uint16_t RTC_COUNT=0;
uint8_t COUNT_NEW = 0;
uint32_t COUNT_DISPLAY = 0;
uint32_t COUNT_NOW = 0;
uint32_t COUNT_PREVIOUS = 0;
ISR(RTC_CNT_vect){
	
	// set PWM level for analog output
	if (IsPlaying()){
		DAC0.DATA = GetNextAudioLevel() << 8;
	}
	
	// count ticks to gate counter
	RTC_COUNT++;
	if (RTC_COUNT == 800){
		RTC_COUNT = 0;
		TCD0.CTRLE = TCD_SCAPTUREA_bm;
		
		// Normally we would wait for sync, but this hangs infinitely if EXTCLK has no signal.
		//while ((TCD0.STATUS & TCD_CMDRDY_bm) == 0); // TODO: timeout if EXTCLK not present
		
		// Instead, just wait a bit for sync to happen.
		_delay_us(100);
		
		COUNT_NOW = COUNTER + TCD0.CAPTUREA;
		if (COUNT_NOW < COUNT_PREVIOUS){
			uint32_t diff = 0 - COUNT_PREVIOUS;
			COUNT_NOW += diff;
		}
		COUNT_DISPLAY = COUNT_NOW - COUNT_PREVIOUS;
		COUNT_DISPLAY = COUNT_DISPLAY * 10;
		COUNT_PREVIOUS = COUNT_NOW;
		COUNT_NEW = 1;
	}

	
	RTC.INTFLAGS = 0x11; // interrupt handled
}

void setup_system_clock_24MHz(){
	// Use the highest frequency system clock (24 MHz)
	CCP = CCP_IOREG_gc;
	CLKCTRL.OSCHFCTRLA = CLKCTRL_FRQSEL_24M_gc;
}

void setup_DAC(){
	DAC0.CTRLA = DAC_OUTEN_bm | DAC_ENABLE_bm;
}

void setup_extclk_counter(){
	// Enable the highest frequency external clock on pin 30
	CCP = CCP_IOREG_gc;
	CLKCTRL.XOSCHFCTRLA = CLKCTRL_FRQRANGE_32M_gc| CLKCTRL_SELHF_bm | CLKCTRL_ENABLE_bm;
	
	// Setup TCD to count the external clock
	TCD0.CMPBCLR = 0x0FFF; // count to max
	TCD0.CTRLA = TCD_CLKSEL_EXTCLK_gc; // count external clock input
	TCD0.INTCTRL = TCD_OVF_bm; // Enable overflow interrupt
	while (!(TCD0.STATUS & 0x01)); // ENRDY
	TCD0.CTRLA |= TCD_ENABLE_bm; // EXTCLK, enable
}

void setup_rtc_gate(){
	// Enable the RTC
	CCP = CCP_IOREG_gc;
	CLKCTRL.XOSC32KCTRLA = CLKCTRL_SEL_bm | CLKCTRL_ENABLE_bm; // External clock on the XTAL32K1 pin, enable
	
	// Setup the RTC at 10 MHz to interrupt periodically
	// 10 MHz with 128 prescaler is 78,125 ticks/sec
	RTC.CTRLA = RTC_PRESCALER_DIV1_gc | RTC_RTCEN_bm;
	RTC.PER = 1250-1; // 8kHz
	RTC.INTCTRL = RTC_OVF_bm;
	RTC.CLKSEL = RTC_CLKSEL_XTAL32K_gc; // clock in XOSC23K pin
}

int main(void)
{
	leds.setup();
	leds.status_on();
	setup_system_clock_24MHz();
	setup_extclk_counter();
	setup_DAC();
	setup_rtc_gate();
	serial.setup();
	button.setup();
	sei(); // Enable global interrupts
	
	_delay_ms(100);
	leds.power_on();
	leds.status_off();
	
	while (1){
		
		while(!COUNT_NEW);
		uint32_t count = COUNT_DISPLAY;
		COUNT_NEW = 0;
		
		if (count > 0){
			leds.signal_on();
			} else {
			leds.signal_off();
		}
		
		//serial.write_number_with_commas(count);
		
		if (button.is_down()){
			leds.status_on();
			serial.write_number_with_commas(count);
			speak_mhz(count, 3);
			leds.status_off();
		}
	}
}
