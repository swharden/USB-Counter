#ifndef SERIALMANAGER_H_
#define SERIALMANAGER_H_

#include <string.h>
#include <stdio.h>

#define USART0_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)

class SerialManager{
	public:
	SerialManager(){
		
	}
	
	void setup(){
		// use alternate pins (TX/RX on PA4/PA5)
		PORTMUX_USARTROUTEA = PORTMUX_USART0_ALT1_gc;
		
		// set TX pin as an output: PA4 (pin2)
		PORTA.DIRSET = PIN4_bm;

		// set baud rate based on CPU clock
		USART0.BAUD = (uint16_t)USART0_BAUD_RATE(9600);
		
		// enable USART TX
		USART0.CTRLB |= USART_TXEN_bm;
	}

	void write_number_with_commas(unsigned long freq){
		
		if (freq == 0){
			write_char('0');
			write_line_break();
			return;
		}
		
		char chars[10];
		
		for (int i=0; i<10; i++){
			if (freq > 0){
				char one = freq % 10;
				chars[10 - 1 - i] = '0' + one;
				freq /= 10;
				} else {
				chars[10 - 1 - i] = ' ';
			}
		}
		
		char number_seen = 0;
		
		for (int i=0; i<10; i++){
			if (number_seen == 0 && chars[i] == ' '){
				continue;
			}
			write_char(chars[i]);
			if ((i%3 == 0) && (i != 9)){
				write_char(',');
			}
		}
		
		write_line_break();
	}
	
	private:

	static void write_line_break(){
		write_char('\r');
		write_char('\n');
	}

	static void write_char(char c)
	{
		while (!(USART0.STATUS & USART_DREIF_bm)) {;}
		USART0.TXDATAL = c;
	}

	void write_string(char *str)
	{
		for(size_t i = 0; i < strlen(str); i++)
		{
			write_char(str[i]);
		}
	}
};

#endif