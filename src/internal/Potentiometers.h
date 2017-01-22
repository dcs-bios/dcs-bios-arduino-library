#ifndef __DCSBIOS_POTS_H
#define __DCSBIOS_POTS_H

#include "Arduino.h"
#include "PollingInput.h"

namespace DcsBios {

	class Potentiometer : PollingInput {
		private:
			void pollInput() {
				unsigned int state = map(analogRead(pin_), 0, 1023, 0, 65535);
				if (state != lastState_) {
					char buf[6];
					utoa(state, buf, 10);
					if (sendDcsBiosMessage(msg_, buf))
						lastState_ = state;
				}
			}
			const char* msg_;
			char pin_;
			unsigned int lastState_;
		public:
			Potentiometer(const char* msg, char pin) {
				msg_ = msg;
				pin_ = pin;
				pinMode(pin_, INPUT);
				lastState_ = map(analogRead(pin_), 0, 1023, 0, 65535);
			}
	};

}

#endif
