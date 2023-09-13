/*

AVR64DD32 Frequency Counter

Counter:
The signal to be counted is passed into EXTCLK (pin 30)
and is counted asynchronously using PD0.

Time base:
A 10 MHz time base is passed into XTAL32K1 (pin 20) and
divided down so the counter is gated ten times per second.

Audio output:
Waveforms are generated using DAC (pin 16).

Button:
Pin 25 is pulled high. Ground to indicate a button press.

*/
#define F_CPU 24000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/cpufunc.h>
#include <avr/pgmspace.h>
#include "NumberSpeaker.h"
#include "Serial.h"

#define setup_led() PORTD.DIRSET = PIN7_bm
#define led_on() PORTD.OUTCLR = PIN7_bm
#define led_off() PORTD.OUTSET = PIN7_bm

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

void setup_button(){
	PORTF.DIRCLR = PIN5_bm; // pin 25
	PORTF.PIN5CTRL = PORT_PULLUPEN_bm;
}

uint8_t is_button_down(){
	if (PORTF.IN & PIN5_bm){
		return 0;
		} else {
		return 1;
	}
}

uint8_t is_button_up(){
	if (PORTF.IN & PIN5_bm){
		return 1;
		} else {
		return 0;
	}
}

void wait_for_button_press(){
	while(is_button_up()){} // pin 25
}

void setup_DAC(){
	DAC0.CTRLA = DAC_OUTEN_bm | DAC_ENABLE_bm;
}

int main(void)
{
	setup_system_clock_24MHz();
	setup_serial();
	setup_led();
	setup_extclk_counter();
	setup_rtc_gate();
	setup_DAC();
	setup_button();
	
	sei(); // Enable global interrupts
	
	printf("\r\n");
	printf("\r\n");
	printf("Talking Frequency Counter 2.3");
	printf("\r\n");
	
	led_on();
	speak_digit(2);
	speak_point();
	speak_digit(3);
	_delay_ms(200);
	led_off();
	
	uint8_t count2 = 0;
	
	while (1){
		
		count2 += 1;
		if (count2 >= 10){
			count2 = 0;
		}
		
		while(!COUNT_NEW);
		uint32_t count = COUNT_DISPLAY;
		COUNT_NEW = 0;
		
		if (count > 0){
			led_on();
			} else {
			led_off();
		}
		
		printf("%d ", count2);
		print_with_commas(count);
		
		if (is_button_down()){
			speak_mhz(count, 3);
		}
	}
}
