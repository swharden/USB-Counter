#ifndef LEDMANAGER_H_
#define LEDMANAGER_H_

class LedManager{
	public:

	LedManager(){
	}
	
	void setup(){
		// power
		PORTC.DIRSET = PIN0_bm;
		PORTC.OUTSET = PIN0_bm;
		
		// status
		PORTC.DIRSET = PIN2_bm;
		PORTC.OUTSET = PIN2_bm;
		
		// signal
		PORTC.DIRSET = PIN1_bm;
		PORTC.OUTSET = PIN1_bm;
	}

	void power_on(){
		PORTC.OUTCLR = PIN0_bm;
	}

	void power_off(){
		PORTC.OUTSET = PIN0_bm;
	}

	void status_on(){
		PORTC.OUTCLR = PIN2_bm;
	}

	void status_off(){
		PORTC.OUTSET = PIN2_bm;
	}

	void signal_on(){
		PORTC.OUTCLR = PIN1_bm;
	}

	void signal_off(){
		PORTC.OUTSET = PIN1_bm;
	}
};

#endif