#include "Arduino.h"
#include "Encoders.h"

namespace DcsBios {

	RotaryEncoder::RotaryEncoder(const char* msg, const char* decArg, const char* incArg, char pinA, char pinB) {
		msg_ = msg;
		decArg_ = decArg;
		incArg_ = incArg;
		pinA_ = pinA;
		pinB_ = pinB;
		pinMode(pinA_, INPUT_PULLUP);
		pinMode(pinB_, INPUT_PULLUP);
		delta_ = 0;
		lastState_ = readState();
	}
	char RotaryEncoder::readState() {
		return (digitalRead(pinA_) << 1) | digitalRead(pinB_);
	}
	/*
	Rotary Encoder state transitions for one step: 
	3 <-> 2 <-> 0 <-> 1 <-> 3
	Right: 11 (3) -> 10 (2) -> 00 (0) -> 01 (1) -> 11 (3)
	Left:  11 (3) -> 01 (1) -> 00 (0) -> 10 (2) -> 11 (3)    
	*/
	void RotaryEncoder::pollInput() {
		char state = readState();
		switch(lastState_) {
			case 0:
				if (state == 2) delta_--;
				if (state == 1) delta_++;
				break;
			case 1:
				if (state == 0) delta_--;
				if (state == 3) delta_++;
				break;
			case 2:
				if (state == 3) delta_--;
				if (state == 0) delta_++;
				break;
			case 3:
				if (state == 1) delta_--;
				if (state == 2) delta_++;
				break;
		}
		lastState_ = state;
		
		if (delta_ == 4) {
			sendDcsBiosMessage(msg_, incArg_);
			delta_ = 0;
		}
		if (delta_ == -4) {
			sendDcsBiosMessage(msg_, decArg_);
			delta_ = 0;
		}
	}

}