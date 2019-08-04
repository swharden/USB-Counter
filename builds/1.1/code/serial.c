#include <avr/io.h>

#define USART_BAUDRATE 115200
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void SerialInitialize()
{
    UBRR0 = UBRR_VALUE;
    UCSR0B |= (1 << TXEN0);                  // enable TX
    UCSR0B |= (1 << RXEN0);                  // enable RX
    UCSR0B |= (1 << RXCIE0);                 // RX complete interrupt
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ01); // no parity, 1 stop bit, 8-bit data
    DDRD |= (1 << PD7);                      // make DSR pin an output
}

void SerialSend(char data)
{
    SerialWaitForSendToComplete();
    UDR0 = data;
}

void SerialWaitForSendToComplete()
{
    while (!(UCSR0A & (1 << UDRE0)))
    {
    };
}

void SerialSendSpace()
{
    SerialSend(' ');
}

void SerialSendComma()
{
    SerialSend(',');
}

void SerialSendPeriod()
{
    SerialSend('.');
}

void SerialSendBreak()
{
    SerialSend(10); // new line
    SerialSend(13); // carriage return
}

void SerialSendNull(char count)
{
    while (count--)
        SerialSend(0);
}

void SerialSendString(const char *s)
{
    while (*s)
        SerialSend(*s++);
}

void SerialSend_UINT32(uint32_t val)
{
    // send a 32-bit (4-byte) long as ASCII
    uint32_t divby = 1000000000;
    while (divby >= 1)
    {
        SerialSend('0' + val / divby);
        val -= (val / divby) * divby;
        divby /= 10;
    }
}

void SerialSend_intThreeDigit(int val)
{
    int divby = 100;
    while (divby >= 1)
    {
        SerialSend('0' + val / divby);
        val -= (val / divby) * divby;
        divby /= 10;
    }
}

void SendStartupMessage()
{

	SerialSendBreak();
	SerialSendBreak();
	SerialSendString("USB Counter - www.SWHarden.com");
	SerialSendBreak();
}