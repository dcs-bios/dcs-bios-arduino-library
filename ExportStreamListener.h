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
				if ((address >= address_) && (endAddress_ > address)) {
					unsigned int index = address - address_;
					buffer[index] = ((char*)&value)[0];
					// No reason to check index here. We allocate an extra byte for zero termination
					// and Export protocol pads odd length strings with a 0.
					index++;
					if (LENGTH > index) {
						buffer[index] = ((char*)&value)[1];
					}
					// No need to compare existing buffer with current value.  We never get to this
					// point unless the sim has sent a change.
					dirty_ = true;
				}
			}
			unsigned int address_;
			unsigned int endAddress_;
			bool dirty_;
		public:
			char buffer[LENGTH+1];
			StringBuffer(unsigned int address) {
				dirty_ = false;
				address_ = address;
				// Move calculating end address into startup.  Timing for
				// parsing loop is more critical than the extra 2 bytes of ram.
				endAddress_ = address+LENGTH;
				memset(buffer, '\0', LENGTH+1);
			}
			// Replace callback with external dirty flag.  Callbacks are
			// not safe inside protcol parsing due to timing criticallity.
			bool isDirty() {
				return dirty_;
			}
			void clearDirty() {
				dirty_ = false;
			}
	};

	class IntegerBuffer : ExportStreamListener {
		private:
			void onDcsBiosWrite(unsigned int address, unsigned int value) {
				if (address == address_) {
					data = (value & mask_) >> shift_;
				}
			}
			unsigned int address_;
			unsigned int mask_;
			unsigned char shift_;
			bool dirty_;
		public:
			int data;
			IntegerBuffer(unsigned int address, unsigned int mask, unsigned char shift) {
				dirty_ = false;
				address_ = address;
				mask_ = mask;
				shift_ = shift;
			}
			bool isDirty() {
				return dirty_;
			}
			void clearDirty() {
				dirty_ = false;
			}
	};
	
}

#endif