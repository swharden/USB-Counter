#ifndef BUTTONMANAGER_H_
#define BUTTONMANAGER_H_

class  ButtonManager
{
	public:
	ButtonManager(){
	}
	
	void setup(){
		PORTD.DIRCLR = PIN5_bm;
		PORTD.PIN5CTRL = PORT_PULLUPEN_bm;
	}
	
	uint8_t is_down(){
		if (PORTD.IN & PIN5_bm){
			return 0;
			} else {
			return 1;
		}
	}
};



#endif