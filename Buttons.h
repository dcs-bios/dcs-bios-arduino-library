#ifndef __DCSBIOS_BUTTONS_H
#define __DCSBIOS_BUTTONS_H

#include "Arduino.h"
#include "PollingInput.h"

namespace DcsBios {
	class ActionButton : PollingInput {
		private:
			void pollInput();
			char* msg_;
			char* arg_;
			char pin_;
			char lastState_;
		public:
			ActionButton(char* msg, char* arg, char pin);
	};
}

#endif