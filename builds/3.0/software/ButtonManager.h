#ifndef BUTTONMANAGER_H_
#define BUTTONMANAGER_H_

class  ButtonManager
{
	public:
	ButtonManager(){
	}
	
	void setup(){
		PORTC.DIRCLR = PIN3_bm;
		PORTC.PIN3CTRL = PORT_PULLUPEN_bm;
	}
	
	uint8_t is_down(){
		if (PORTC.IN & PIN3_bm){
			return 0;
			} else {
			return 1;
		}
	}
};



#endif