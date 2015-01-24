#ifndef __DCSBIOS_POLLINGINPUT_H
#define __DCSBIOS_POLLINGINPUT_H

#include "Arduino.h"

void sendDcsBiosMessage(const char* msg, const char* args);

namespace DcsBios {

	class PollingInput {
		private:
			virtual void pollInput();
			PollingInput* nextPollingInput;
		public:
			static PollingInput* firstPollingInput;
			PollingInput() {
				this->nextPollingInput = firstPollingInput;
				firstPollingInput = this;
			}
			static void pollInputs() {
				PollingInput* pi = firstPollingInput;
				while (pi) {
					pi->pollInput();
					pi = pi->nextPollingInput;
				}
			}
	};

}

#endif