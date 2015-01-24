#ifndef __DCSBIOS_POTS_H
#define __DCSBIOS_POTS_H

#include "Arduino.h"
#include "PollingInput.h"

namespace DcsBios {

	class Potentiometer : PollingInput {
		private:
			void pollInput();
			char* msg_;
			char pin_;
			unsigned int lastState_;
		public:
			Potentiometer(char* msg, char pin);
	};

}

#endif