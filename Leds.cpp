#include "Arduino.h"
#include "Leds.h"

namespace DcsBios {

	LED::LED(unsigned int address, unsigned int mask, char pin) : IntegerData(address, mask, 0) {
		pin_ = pin;
		pinMode(pin_, OUTPUT);
		digitalWrite(pin_, LOW);
	}
	void LED::onDcsBiosFrameSync() {
		if (data) {
			digitalWrite(pin_, HIGH);
		} else {
			digitalWrite(pin_, LOW);
		}			
	}

}