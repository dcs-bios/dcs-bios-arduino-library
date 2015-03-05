#ifndef __DCSBIOS_SERVOS_H
#define __DCSBIOS_SERVOS_H

#include "Arduino.h"
#include "ExportStreamListener.h"
#include <Servo.h>

namespace DcsBios {
	class ServoOutput : IntegerData {
		private:
			void onDcsBiosFrameSync();
			Servo servo_;
			char pin_;
			int minPulseWidth_;
			int maxPulseWidth_;
		public:
			ServoOutput(unsigned int address, char pin, int minPulseWidth, int maxPulseWidth);
			ServoOutput(unsigned int address, char pin);
	};
}

#endif