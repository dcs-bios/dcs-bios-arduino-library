#include "Arduino.h"
#include "Leds.h"

namespace DcsBios {

	LED::LED(unsigned int address, unsigned int mask, char pin) {
		address_ = address;
		mask_ = mask;
		pin_ = pin;
		pinMode(pin_, OUTPUT);
		digitalWrite(pin_, LOW);
	}
	void LED::onDcsBiosWrite(unsigned int address, unsigned int value) {
		if (address_ == address) {
			if (value & mask_) {
				digitalWrite(pin_, HIGH);
			} else {
				digitalWrite(pin_, LOW);
			}
		}
	}

}