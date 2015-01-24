#include "Arduino.h"
#include "Switches.h"

namespace DcsBios {
	Switch2Pos::Switch2Pos(char* msg, char pin) {
		msg_ = msg;
		pin_ = pin;
		pinMode(pin_, INPUT_PULLUP);
		lastState_ = digitalRead(pin_);
	}
	void Switch2Pos::pollInput() {
		char state = digitalRead(pin_);
		if (state != lastState_) {
			sendDcsBiosMessage(msg_, state == HIGH ? "0" : "1");
		}
		lastState_ = state;
	}

	Switch3Pos::Switch3Pos(char* msg, char pinA, char pinB) {
		msg_ = msg;
		pinA_ = pinA;
		pinB_ = pinB;
		pinMode(pinA_, INPUT_PULLUP);
		pinMode(pinB_, INPUT_PULLUP);
		lastState_ = readState();
	}
	char Switch3Pos::readState() {
		if (digitalRead(pinA_) == LOW) return 0;
		if (digitalRead(pinB_) == LOW) return 2;
		return 1;
	}
	void Switch3Pos::pollInput() {
		char state = readState();
		if (state != lastState_) {
			if (state == 0)
				sendDcsBiosMessage(msg_, "0");
			if (state == 1)
				sendDcsBiosMessage(msg_, "1");
			if (state == 2)
				sendDcsBiosMessage(msg_, "2");
		}
		lastState_ = state;
	}	

	SwitchMultiPos::SwitchMultiPos(char* msg, const byte* pins, char numberOfPins) {
		msg_ = msg;
		pins_ = pins;
		numberOfPins_ = numberOfPins;
		char i;
		for (i=0; i<numberOfPins; i++) {
			pinMode(pins[i], INPUT_PULLUP);
		}
		lastState_ = readState();
	}
	char SwitchMultiPos::readState() {
		char i;
		for (i=0; i<numberOfPins_; i++) {
			if (digitalRead(pins_[i]) == LOW) return i;
		}
		return 0;
	}
	void SwitchMultiPos::pollInput() {
		char state = readState();
		if (state != lastState_) {
			char buf[7];
			utoa(state, buf, 10);
			sendDcsBiosMessage(msg_, buf);
		}
		lastState_ = state;
	}	
}