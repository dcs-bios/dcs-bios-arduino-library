#include "Arduino.h"
#include "Potentiometers.h"

namespace DcsBios {
	
	Potentiometer::Potentiometer(char* msg, char pin, unsigned int deadband) {
		msg_ = msg;
		pin_ = pin;
		deadBand_ = deadband;
		pinMode(pin_, INPUT);
		lastState_ = map(analogRead(pin_), 0, 1023, 0, 65535);
	}
	void Potentiometer::pollInput() {
		unsigned int state = map(analogRead(pin_), 0, 1023, 0, 65535);

		// Work out the clipped lower/upper boundaries and clip them to the limits
		unsigned int lowerBound = ( lastState_ < deadBand_ )?0:( lastState_-deadBand_ );
		unsigned int upperBound = ( lastState_ > ( 65535 - deadBand_ ))?65535:( lastState_+deadBand_ );
		
		if (( state < lowerBound ) || ( state > upperBound )) {
			char buf[6];
			utoa(state, buf, 10);
			sendDcsBiosMessage(msg_, buf);
			lastState_ = state;
		}
	}

}
