/* Code here relates to use of 16-bit timer for timing purposes */

volatile int overflows;

void TimerInitialize1Hz()
{
	// The CPU clock produces 14,745,600 ticks/second
	// prescaled by 1024 that's 14,400 ticks/second

	int updatesPerSecond = 100;

	TCCR1B |= _BV(WGM13);				 // mode 8 (ICR1 top)
	TCCR1B |= _BV(CS12) | _BV(CS10);	 // prescale by 1024
	TIMSK1 |= _BV(TOIE1);				 // enable overflow interrupt
	ICR1 = 14400 / 2 / updatesPerSecond; // why div2?
	sei();								 // enable global interrupts
}

volatile char gateCycled = 0;
volatile int seconds = 0;
volatile int milliseconds = 0;

ISR(TIMER1_OVF_vect)
{
	DDRB |= (1 << PB2); // set gate as output

	PORTB &= ~_BV(PB2); // cycle the gate
	PORTB |= _BV(PB2);

	milliseconds += 10;
	if (milliseconds >= 1000)
	{
		milliseconds = 0;
		seconds += 1;
		if (seconds >= 1000)
			seconds = 0;
	}
	gateCycled = 1;
}