#ifndef __DCSBIOS_SWITCHES_H
#define __DCSBIOS_SWITCHES_H

#include "Arduino.h"
#include "PollingInput.h"

namespace DcsBios {

	class Switch2Pos : PollingInput {
		private:
			void pollInput();
			char* msg_;
			char pin_;
			char lastState_;
		public:
			Switch2Pos(char* msg, char pin);
	};

	class Switch3Pos : PollingInput {
		private:
			void pollInput();
			char* msg_;
			char pinA_;
			char pinB_;
			char lastState_;
			char readState();
		public:
			Switch3Pos(char* msg, char pinA, char pinB);
	};

	class SwitchMultiPos : PollingInput {
		private:
			void pollInput();
			char* msg_;
			const byte* pins_;
			char numberOfPins_;
			char lastState_;
			char readState();
		public:
			SwitchMultiPos(char* msg_, const byte* pins, char numberOfPins);
	};

}

#endif	