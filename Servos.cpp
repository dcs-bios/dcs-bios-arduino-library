#include "Arduino.h"
#include "Servos.h"

namespace DcsBios {

	ServoOutput::ServoOutput(unsigned int address, char pin, int minPulseWidth, int maxPulseWidth) {
		address_ = address;
		pin_ = pin;
		minPulseWidth_ = minPulseWidth;
		maxPulseWidth_ = maxPulseWidth;
	}
	void ServoOutput::onDcsBiosWrite(unsigned int address, unsigned int value) {
		if (address_ == address) {
			if (!servo_.attached())
				servo_.attach(pin_, minPulseWidth_, maxPulseWidth_);
			servo_.writeMicroseconds(map(value, 0, 65535, minPulseWidth_, maxPulseWidth_));
		}
	}

}