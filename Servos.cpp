#include "Arduino.h"
#include "Servos.h"

namespace DcsBios {

	ServoOutput::ServoOutput(unsigned int address, char pin, int minPulseWidth, int maxPulseWidth) : IntegerBuffer (address, 0xffff, 0) {
		pin_ = pin;
		minPulseWidth_ = minPulseWidth;
		maxPulseWidth_ = maxPulseWidth;
	}
	ServoOutput::ServoOutput(unsigned int address, char pin) : IntegerBuffer (address, 0xffff, 0) {
		ServoOutput(address, pin, 544, 2400);
	}
	void ServoOutput::onDcsBiosFrameSync() {
		if (!servo_.attached())
			servo_.attach(pin_, minPulseWidth_, maxPulseWidth_);
		servo_.writeMicroseconds(map(data, 0, 65535, minPulseWidth_, maxPulseWidth_));
	}

}