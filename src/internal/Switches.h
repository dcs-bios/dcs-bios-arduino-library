#ifndef __DCSBIOS_SWITCHES_H
#define __DCSBIOS_SWITCHES_H

#include "Arduino.h"

namespace DcsBios {

	
	class Switch2Pos : PollingInput {
		private:
			const char* msg_;
			char pin_;
			char lastState_;
			char switchState_;
			bool reverse_;
			unsigned long debounceDelay_; 
			unsigned long lastDebounceTime = 0;
			void init_(const char* msg, char pin, bool reverse, unsigned long debounceDelay) {
				msg_ = msg;
				pin_ = pin;
				pinMode(pin_, INPUT_PULLUP);
				switchState_ = digitalRead(pin_);
				lastState_ = switchState_;
				reverse_ = reverse;
				debounceDelay_ = debounceDelay;
			}
			void pollInput() {
				char state = digitalRead(pin_);
				if (reverse_) state = !state;
				if (state != lastState_) {
					lastDebounceTime = millis();
				}
				
				if ((millis() - lastDebounceTime) > debounceDelay_) {
					if (state != switchState_) {
						if (tryToSendDcsBiosMessage(msg_, state == HIGH ? "0" : "1")) {
							switchState_ = state;
						}
					}
				}
				lastState_ = state;
			}
		public:
			Switch2Pos(const char* msg, char pin, bool reverse, unsigned long debounceDelay) { init_(msg, pin, reverse, debounceDelay); }
			Switch2Pos(const char* msg, char pin, bool reverse) { init_(msg, pin, reverse, 50); }
			Switch2Pos(const char* msg, char pin) { init_(msg, pin, false, 50); }
	};

	
	
	
	class Switch3Pos : PollingInput {
		private:
			const char* msg_;
			char pinA_;
			char pinB_;
			char lastState_;
			char readState() {
				if (digitalRead(pinA_) == LOW) return 0;
				if (digitalRead(pinB_) == LOW) return 2;
				return 1;
			}
			void pollInput() {
				char state = readState();
				if (state != lastState_) {
					if (state == 0)
						if (tryToSendDcsBiosMessage(msg_, "0"))
							lastState_ = state;
					if (state == 1)
						if (tryToSendDcsBiosMessage(msg_, "1"))
							lastState_ = state;
					if (state == 2)
						if(tryToSendDcsBiosMessage(msg_, "2"))
							lastState_ = state;
				}
			}
		public:
			Switch3Pos(const char* msg, char pinA, char pinB) {
				msg_ = msg;
				pinA_ = pinA;
				pinB_ = pinB;
				pinMode(pinA_, INPUT_PULLUP);
				pinMode(pinB_, INPUT_PULLUP);
				lastState_ = readState();
			}
	};

	
	class SwitchMultiPos : PollingInput {
		private:
			const char* msg_;
			const byte* pins_;
			char numberOfPins_;
			char lastState_;
			char readState() {
				unsigned char i;
				for (i=0; i<numberOfPins_; i++) {
					if (digitalRead(pins_[i]) == LOW) return i;
				}
				return lastState_;
			}
			void pollInput() {
				char state = readState();
				if (state != lastState_) {
					char buf[7];
					utoa(state, buf, 10);
					if (tryToSendDcsBiosMessage(msg_, buf))
						lastState_ = state;
				}
			}
		public:
			SwitchMultiPos(const char* msg, const byte* pins, char numberOfPins) : lastState_(0) {
				msg_ = msg;
				pins_ = pins;
				numberOfPins_ = numberOfPins;
				unsigned char i;
				for (i=0; i<numberOfPins; i++) {
					pinMode(pins[i], INPUT_PULLUP);
				}
				lastState_ = readState();
			}
	};

}

#endif	