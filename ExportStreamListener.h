#ifndef __DCSBIOS_EXPORTSTREAMLISTENER_H
#define __DCSBIOS_EXPORTSTREAMLISTENER_H

#include "Arduino.h"

namespace DcsBios {
	class ExportStreamListener {
		private:
			virtual void onDcsBiosWrite(unsigned int address, unsigned int value) {}
			virtual void onDcsBiosFrameSync() {}
			ExportStreamListener* nextExportStreamListener;
		public:
			static ExportStreamListener* firstExportStreamListener;
			ExportStreamListener() {
				this->nextExportStreamListener = firstExportStreamListener;
				firstExportStreamListener = this;
			}
			static void handleDcsBiosWrite(unsigned int address, unsigned int value) {
				ExportStreamListener* el = firstExportStreamListener;
				while (el) {
					el->onDcsBiosWrite(address, value);
					el = el->nextExportStreamListener;
				}
			}
			static void handleDcsBiosFrameSync() {
				ExportStreamListener* el = firstExportStreamListener;
				while (el) {
					el->onDcsBiosFrameSync();
					el = el->nextExportStreamListener;
				}
			}
	};

	template < unsigned int LENGTH >
	class StringBuffer : ExportStreamListener {
		private:
			void onDcsBiosWrite(unsigned int address, unsigned int value) {
				if ((address >= address_) && (address_ + LENGTH > address)) {
					setChar(address - address_, ((char*)&value)[0]);
				if (address_ + LENGTH > (address+1))
					setChar(address - address_ + 1, ((char*)&value)[1]);
				}
				if (address == 0xfffe) {
					if (dirty_) {
						callback_(buffer);
						dirty_ = false;
					}
				}
			}
			void setChar(unsigned int index, unsigned char value) {
				if (buffer[index] == value) return;
				buffer[index] = value;
				dirty_ = true;
			}
			unsigned int address_;
			bool dirty_;
			void (*callback_)(char*);
		public:
			char buffer[LENGTH+1];
			StringBuffer(unsigned int address, void (*callback)(char*)) {
				callback_ = callback;
				dirty_ = false;
				address_ = address;
				memset(buffer, '\0', LENGTH+1);
			}
	};
	
}

#endif