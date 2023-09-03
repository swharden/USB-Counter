/* Experimental code for an AVRDD32 */

#define F_CPU 10000000UL
#define USART0_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>

void ccp_write(volatile register8_t* address, uint8_t value){
	CCP = CCP_IOREG_gc;
	*address = value;
}

/* Use the internal 20 MHz oscillator scaled down to 10 MHz */
void clock_setup_internal(){
	ccp_write(&CLKCTRL.MCLKCTRLA, CLKCTRL_CLKOUT_bm); // enable clock out pin
	ccp_write(&CLKCTRL.MCLKCTRLB, CLKCTRL_PEN_bm); // enable prescaler (divide by 2)
	ccp_write(&CLKCTRL.OSCHFCTRLA, CLKCTRL_FRQSEL_20M_gc); // 20MHz internal
}

/* Use an external 10 MHz clock */
void clock_setup_external(){
	ccp_write(&CLKCTRL.MCLKCTRLA, 0b10001000); // enable clock out pin, external clock
	ccp_write(&CLKCTRL.MCLKCTRLB, 0); // disable prescaler
	ccp_write(&CLKCTRL.XOSCHFCTRLA, 0b10011011); // external clock on XTALHF1 pin
}

void status_led_setup(){
	PORTD.DIR = 0xFF;
}

void status_led_on(){
	PORTD.OUT = 0;
}

void status_led_toggle(){
	PORTD.OUT = ~PORTD.OUT;
}

void status_led_off(){
	PORTD.OUT = 255;
}

void serial_setup(void)
{
	PORTA.DIR = 255;
	//PORTMUX_USARTROUTEA = PORTMUX_USART0_ALT1_gc;
	USART0.BAUD = (uint16_t)USART0_BAUD_RATE(9600);
	USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
}

static void serial_send_char(char c)
{
	while (!(USART0.STATUS & USART_DREIF_bm)) {;}
	USART0.TXDATAL = c;
}

void serial_send_string(char *str)
{
	for(size_t i = 0; i < strlen(str); i++)
	{
		serial_send_char(str[i]);
	}
}

int main(void)
{
	clock_setup_internal();
	status_led_setup();
	serial_setup();
	
	char count = 0;
	
	while (1)
	{
		status_led_toggle();
		serial_send_string("test ");
		serial_send_char('0' + count);
		serial_send_string("\r\n");
		
		count += 1;
		if (count >= 10){
			count = 0;
		}
		
		_delay_ms(1000);
	}
}

