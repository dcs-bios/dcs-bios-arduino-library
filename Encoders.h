#ifndef __DCSBIOS_ENCODERS_H
#define __DCSBIOS_ENCODERS_H

#include "Arduino.h"
#include "PollingInput.h"

namespace DcsBios {
	class RotaryEncoder : PollingInput {
		private:
			void pollInput();
			char readState();
			const char* msg_;
			const char* decArg_;
			const char* incArg_;
			char pinA_;
			char pinB_;
			char lastState_;
			signed char delta_;
		public:
			RotaryEncoder(const char* msg, const char* decArg, const char* incArg, char pinA, char pinB);	
	};
}

#endif