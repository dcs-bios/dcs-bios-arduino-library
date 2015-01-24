#ifndef __DCSBIOS_PROTOCOL_H
#define __DCSBIOS_PROTOCOL_H

#define DCSBIOS_STATE_WAIT_FOR_SYNC 0
#define DCSBIOS_STATE_ADDRESS_LOW 1
#define DCSBIOS_STATE_ADDRESS_HIGH 2
#define DCSBIOS_STATE_COUNT_LOW 3
#define DCSBIOS_STATE_COUNT_HIGH 4
#define DCSBIOS_STATE_DATA_LOW 5
#define DCSBIOS_STATE_DATA_HIGH 6

void onDcsBiosWrite(unsigned int address, unsigned int value);

namespace DcsBios {

	class ProtocolParser {
		private:
			unsigned char state;
			unsigned int address;
			unsigned int count;
			unsigned int data;
			unsigned char sync_byte_count;
		public:
			void processChar(unsigned char c);
			ProtocolParser();
	};
}

#endif