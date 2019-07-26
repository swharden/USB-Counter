/* Code here relates to use of 16-bit timer for timing purposes */

volatile int overflows;

void TimerInitialize1Hz()
{
	// at 10MHz ther eare 10,000,000 ticks/second
	// prescaled to 64 = 156,250 ticks/second = 31,250 ticks every 200ms
	// prescaled to 64 = 156,250 ticks/second = 15,625 ticks every 100ms

	TCCR1B |= _BV(WGM13);			 // mode 8 (ICR1 top)
	TCCR1B |= _BV(CS11) | _BV(CS10); // prescale by 64
	TIMSK1 |= _BV(TOIE1);			 // enable overflow interrupt
	ICR1 = 31250 / 2;				 // why div2?
	sei();							 // enable global interrupts
}

volatile char gateCycled = 0;

ISR(TIMER1_OVF_vect)
{
	DDRB |= (1 << PB2); // set gate as output
	overflows++;
	if (overflows == 5)
	{
		PORTB |= _BV(PB2);
		gateCycled = 1;
		overflows = 0;
	}
	else
	{
		PORTB &= ~_BV(PB2);
	}
}