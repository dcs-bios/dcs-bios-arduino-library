#include "Arduino.h"
#include "Buttons.h"

namespace DcsBios {
	ActionButton::ActionButton(char* msg, char* arg, char pin) {
		msg_ = msg;
		arg_ = arg;
		pin_ = pin;
		pinMode(pin_, INPUT_PULLUP);
		lastState_ = digitalRead(pin_);
	}
	void ActionButton::pollInput() {
		char state = digitalRead(pin_);
		if (state != lastState_) {
			if (lastState_ == HIGH && state == LOW) {
				sendDcsBiosMessage(msg_, arg_);
			}
		}
		lastState_ = state;
	}
}