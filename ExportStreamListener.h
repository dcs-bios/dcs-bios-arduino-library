#ifndef __DCSBIOS_EXPORTSTREAMLISTENER_H
#define __DCSBIOS_EXPORTSTREAMLISTENER_H

#include "Arduino.h"

namespace DcsBios {
	class ExportStreamListener {
		protected:
			uint16_t firstAddressOfInterest;
			uint16_t lastAddressOfInterest;
		public:
			virtual void onDcsBiosWrite(unsigned int address, unsigned int value) {}
			virtual void onConsistentData() {}
			ExportStreamListener* nextExportStreamListener;
			inline uint16_t getFirstAddressOfInterest() { return firstAddressOfInterest; }
			inline uint16_t getLastAddressOfInterest() { return lastAddressOfInterest; }
			
			static ExportStreamListener* firstExportStreamListener;
			ExportStreamListener(uint16_t firstAddressOfInterest, uint16_t lastAddressOfInterest) {
				this->firstAddressOfInterest = firstAddressOfInterest;
				this->lastAddressOfInterest = lastAddressOfInterest;
				
				// nothing in the list? insert self as first element.
				if (firstExportStreamListener == NULL) {
					firstExportStreamListener = this;
					nextExportStreamListener = NULL;
					return;
				}
				
				// insert into list of export stream listeners,
				// keep list ordered ascending by lastAddressOfInterest
				ExportStreamListener** prevNextPtr = &firstExportStreamListener;
				ExportStreamListener* nextESL = firstExportStreamListener;
				
				while (nextESL && nextESL->getLastAddressOfInterest() < lastAddressOfInterest) {
					prevNextPtr = &(nextESL->nextExportStreamListener);
					nextESL = nextESL->nextExportStreamListener;
				}
				this->nextExportStreamListener = nextESL;
				*prevNextPtr = this;
				
			}
			static void loopAll() {
				ExportStreamListener* el = firstExportStreamListener;
				while (el) {
					el->loop();
					el = el->nextExportStreamListener;
				}
			}
			virtual void loop() {}
	};

	class Int16Buffer : public ExportStreamListener {
		private:
			volatile unsigned int data;
			volatile bool dirty;
		public:
			Int16Buffer(unsigned int address) : ExportStreamListener(address, address) {
			
			}
			virtual void onDcsBiosWrite(unsigned int address, unsigned int data) {

				this->data = data;
				this->dirty = true;
			
			}
			bool hasUpdatedData() { return dirty; }
			unsigned int getData() {
				uint16_t ret;
				noInterrupts();
				ret = data;
				dirty = false;
				interrupts();
				
				return ret;
			}
	};
	
	template < unsigned int LENGTH >
	class StringBuffer : public ExportStreamListener {
		private:
			char receivingBuffer[LENGTH+1];
			char userBuffer[LENGTH+1];
			volatile bool receivingDirty;
			bool userDirty;
			void (*callback)(char*);
			void setChar(unsigned int index, unsigned char value) {
				// DCS-BIOS will occasionally send data even if it did not change
				if (receivingBuffer[index] == value) return;
				
				receivingBuffer[index] = value;
				receivingDirty = true;
			}
		public:
			StringBuffer(unsigned int address, void (*callback)(char*)) : ExportStreamListener(address, address+LENGTH) {
				memset(receivingBuffer, ' ', LENGTH-1);
				receivingBuffer[LENGTH] = '\0';
				userBuffer[LENGTH] = '\0';
				
				receivingDirty = false;
				userDirty = false;
				this->callback = callback;
			}
			virtual void onDcsBiosWrite(unsigned int address, unsigned int data) {
				unsigned int index = address - firstAddressOfInterest;
				setChar(index, ((char*)&data)[0] );
				index++;
				if (LENGTH > index) {
					setChar(index, ((char*)&data)[1] );
				}
			}
			virtual void onConsistentData() {
				if (receivingDirty) {
					noInterrupts();
					memcpy(userBuffer, receivingBuffer, LENGTH);
					receivingDirty = false;
					userDirty = true;
					interrupts();
				}
			}
			bool hasUpdatedData() {
				return userDirty;
			}
			char* getData() {
				userDirty = false;
				return userBuffer;
			}
			virtual void loop() {
				if (hasUpdatedData()) {
					if (callback) {
						callback(getData());
					}
				}
			}
	};
	
	/*
	template < unsigned int LENGTH >
	class StringBuffer : ExportStreamListener {
		private:
			void onDcsBiosWrite(unsigned int address, unsigned int value) {
				if ((address >= address_) && (endAddress_ > address)) {
					unsigned int index = address - address_;
					buffer[index] = ((char*)&value)[0];
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
	*/
/*
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
	*/
}

#endif