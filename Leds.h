#ifndef __DCSBIOS_LEDS_H
#define __DCSBIOS_LEDS_H

#include "Arduino.h"
#include "ExportStreamListener.h"

namespace DcsBios {

	class LED : ExportStreamListener {
		private:
			void onDcsBiosWrite(unsigned int address, unsigned int value);
			unsigned char pin_;
			unsigned int address_;
			unsigned int mask_;
		public:
			LED(unsigned int address, unsigned int mask, char pin);
	};

}

#endif