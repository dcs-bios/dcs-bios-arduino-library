#ifndef __DCSBIOS_LEDS_H
#define __DCSBIOS_LEDS_H

#include "Arduino.h"
#include "ExportStreamListener.h"

namespace DcsBios {

	class LED : IntegerBuffer {
		private:
			void onDcsBiosFrameSync();
			unsigned char pin_;
		public:
			LED(unsigned int address, unsigned int mask, char pin);
	};

}

#endif