#ifndef __DCSBIOS_SERVOS_H
#define __DCSBIOS_SERVOS_H

#include "Arduino.h"
#include "ExportStreamListener.h"
#include <Servo.h>

namespace DcsBios {
	class ServoOutput : ExportStreamListener {
		private:
			void onDcsBiosWrite(unsigned int address, unsigned int value);
			Servo servo_;
			unsigned int address_;
			char pin_;
			int minPulseWidth_;
			int maxPulseWidth_;
		public:
			ServoOutput(unsigned int address, char pin, int minPulseWidth, int maxPulseWidth);
			ServoOutput(unsigned int address, char pin) { ServoOutput(address, pin, 544, 2400); }
	};
}

#endif