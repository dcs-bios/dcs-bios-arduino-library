#include "Arduino.h"
#include "Potentiometers.h"

namespace DcsBios {
	
	Potentiometer::Potentiometer(char* msg, char pin) {
		msg_ = msg;
		pin_ = pin;
		pinMode(pin_, INPUT);
		lastState_ = map(analogRead(pin_), 0, 1023, 0, 65535);
	}
	void Potentiometer::pollInput() {
		unsigned int state = map(analogRead(pin_), 0, 1023, 0, 65535);
		if (state != lastState_) {
			char buf[6];
			utoa(state, buf, 10);
			sendDcsBiosMessage(msg_, buf);
		}
		lastState_ = state;
	}

}