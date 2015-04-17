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
			unsigned int deadBand_;
			bool	invert_;
		public:
			Potentiometer(char* msg, char pin, unsigned int deadband=0, bool invert=false);
	};

}

#endif