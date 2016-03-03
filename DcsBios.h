#ifndef __DCSBIOS_H
#define __DCSBIOS_H

#ifndef NULL
#define NULL 0
#endif

#include <stdint.h>

#include "ExportStreamListener.h"
#include "PollingInput.h"
#include "Protocol.h"

/*
The following is an ugly hack to work with the Arduino IDE's build system.
The DCS-BIOS Arduino Library is configured with #defines such as DCSBIOS_RS485_MASTER or DCSBIOS_RS485_SLAVE <address>.
To make sure these defines are visible when compiling the code, we can't put it into a separate translation unit.

Normally, those #defines would go in a separate "config.h" or you would use compiler flags. But since Arduino libraries
do not come with their own build system, we are just putting everything into the header file.
*/
#ifdef DCSBIOS_RS485_MASTER
	#include "DcsBiosNgRS485Master.h"
	#include "DcsBiosNgRS485Master.cpp.inc"
#endif
#ifdef DCSBIOS_RS485_SLAVE
	#include "DcsBiosNgRS485Slave.h"
	#include "DcsBiosNgRS485Slave.cpp.inc"
#endif
#ifdef DCSBIOS_IRQ_SERIAL

	#ifndef USART0_RX_vect
	#define USART0_RX_vect USART_RX_vect
	#define PRR0 PRR
	#endif

	namespace DcsBios {
		ProtocolParser parser;
		
		ISR(USART0_RX_vect) {
			volatile uint8_t c = UDR0;
			parser.processCharISR(c);
		}
		
		void setup() {
			PRR0 &= ~(1<<PRUSART0);
			UBRR0H = 0;
			UBRR0L = 3; // 250000 bps
			UCSR0A = 0;
			UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
			
			UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
			
			
		}
		
		void loop() {
			PollingInput::pollInputs();
			ExportStreamListener::loopAll();
		}
		
		static void usart_tx(const char* str) {
			const char* c = str;
			while (*c) {
				while(!(UCSR0A & (1<<UDRE0))); // wait until TX buffer is empty
				UDR0 = *c++; // write byte to TX buffer
			}
		}
	}
	bool sendDcsBiosMessage(const char* msg, const char* arg) {
		DcsBios::usart_tx(msg);
		DcsBios::usart_tx(" ");
		DcsBios::usart_tx(arg);
		DcsBios::usart_tx("\n");
		return true;
	}
#endif
#ifdef DCSBIOS_DEFAULT_SERIAL
	namespace DcsBios {
		ProtocolParser parser;
		void setup() {
			Serial.begin(250000);
		}
		void loop() {
			while (Serial.available()) {
				parser.processChar(Serial.read());
			}
			PollingInput::pollInputs();
			ExportStreamListener::loopAll();			
		}
	}
	bool sendDcsBiosMessage(const char* msg, const char* arg) {
		Serial.write(msg); Serial.write(' '); Serial.write(arg); Serial.write('\n');
	}
#endif


#include "Buttons.h"
#include "Switches.h"
#include "Encoders.h"
#include "Potentiometers.h"
#include "Leds.h"
#include "Servos.h"

#endif