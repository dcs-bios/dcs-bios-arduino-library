#ifdef DCSBIOS_RS485_SLAVE



namespace DcsBios {

	RS485Slave::RS485Slave(volatile uint8_t *udr, volatile uint8_t *ucsra, volatile uint8_t *ucsrb, volatile uint8_t *ucsrc, uint8_t txen_pin) :
	udr(udr), ucsra(ucsra), ucsrb(ucsrb), ucsrc(ucsrc) {
		txen_port = portOutputRegister(digitalPinToPort(txen_pin));
		txen_pin_mask = digitalPinToBitMask(txen_pin);
		state = UNINITIALIZED;
	}
	
	void RS485Slave::udreISR() {
		switch (state) {
			case TX_DATALENGTH_SENT:
				tx_byte(0); // MSGTYPE
				state = TX;
			break;
			
			case TX:
				if (rxtx_len == 0) {
					tx_byte(0x72); // TODO: transmit actual checksum
					state = TX_CHECKSUM_SENT;
					clear_udrie();
				} else {
					rxtx_len--;
					tx_byte(messageBuffer.get());
				}
			break;
		}
	}
	
	void RS485Slave::txcISR() {
		clear_txen();
		switch(state) {
			case TX_SEND_ZERO_DATALENGTH:
				tx_byte(0);
				state = TX_ZERO_DATALENGTH_SENT;
			break;
			
			case TX_SEND_DATALENGTH:
				tx_byte(rxtx_len);
				state = TX_DATALENGTH_SENT;
				set_udrie();
			break;
			
			case TX_ZERO_DATALENGTH_SENT:
				state = RX_WAIT_ADDRESS;
			break;
			
			case TX_CHECKSUM_SENT:
				state = RX_WAIT_ADDRESS;
				messageBuffer.clear();
				messageBuffer.complete = false;
			break;
		}
	}
	
	void RS485Slave::rxISR() {
		volatile uint8_t c = *udr;
		switch(state) {
			case UNINITIALIZED:
				last_rx_time = micros();
				state = SYNC;
			break;
			
			case SYNC:
				if (micros() - last_rx_time < 500) {
					last_rx_time = micros();
					break;
				} else {
					state = RX_WAIT_ADDRESS;
				}
			// no break statement here, we want it to fall through to the next case!
			
			case RX_WAIT_ADDRESS:
				rx_slave_address = c;
				state = RX_WAIT_MSGTYPE;
			break;
			
			case RX_WAIT_MSGTYPE:
				rx_msgtype = c;
				state = RX_WAIT_DATALENGTH;
			break;
			
			case RX_WAIT_DATALENGTH:
				rxtx_len = c;
				
				if (rxtx_len == 0) {
					state = RX_HOST_MESSAGE_COMPLETE;
					break;
				}
				
				if (rx_slave_address == 0 && rx_msgtype == 0) {
					rx_datatype = RXDATA_DCSBIOS_EXPORT;
				} else {
					rx_datatype = RXDATA_IGNORE;
				}
				
				state = RX_WAIT_DATA;
			break;
			
			case RX_WAIT_DATA:
				rxtx_len--;
				if (rx_datatype == RXDATA_DCSBIOS_EXPORT) {
					parser.processCharISR(c);
				}
				if (rxtx_len == 0) {
					state = RX_WAIT_CHECKSUM;
				}
			break;
			
			case RX_WAIT_CHECKSUM:
				// ignore checksum
				state = RX_HOST_MESSAGE_COMPLETE;
			break;
			
			case RX_WAIT_ANSWER_DATALENGTH:
				rxtx_len = c;
				if (rxtx_len == 0) {
					state = RX_WAIT_ADDRESS;
				} else {
					state = RX_WAIT_ANSWER_MSGTYPE;
				}
			break;
			
			case RX_WAIT_ANSWER_MSGTYPE:
				rx_msgtype = c;
				state = RX_WAIT_ANSWER_DATA;
			break;
			
			case RX_WAIT_ANSWER_DATA:
				rxtx_len--;
				if (rxtx_len == 0) {
					state = RX_WAIT_ANSWER_CHECKSUM;
				}
			break;
			
			case RX_WAIT_ANSWER_CHECKSUM:
				// ignore checksum
				state = RX_WAIT_ADDRESS;
			break;
		}
		
		if (state == RX_HOST_MESSAGE_COMPLETE) {
			if (rx_slave_address == 0) {
				// messages to address zero must not be answered by anyone
				state = RX_WAIT_ADDRESS;
			} else if (rx_slave_address == DCSBIOS_RS485_SLAVE) {
				// we have to answer this message
				if (rx_msgtype == 0 && rxtx_len == 0) {
					// we have just been polled for DCS-BIOS import data...
					if (!messageBuffer.complete) {
						// ...but we have nothing to say
						tx_delay_byte();
						state = TX_SEND_ZERO_DATALENGTH;
					} else {
						// ...and we have something to transmit
						rxtx_len = messageBuffer.getLength();
						tx_delay_byte();
						state = TX_SEND_DATALENGTH;
					}
				} else {
					// we have been sent a message that we did not expect
					last_rx_time = micros();
					state = SYNC;
				}
			} else {
				// this message is for someone else
				state = RX_WAIT_ANSWER_DATALENGTH;
			}
		}
		
	}

	
	RS485Slave rs485slave(&UDR0, &UCSR0A, &UCSR0B, &UCSR0C, TXENABLE_PIN);	
	ISR(USART0_RX_vect) { rs485slave.rxISR(); }
	ISR(USART0_TX_vect) { rs485slave.txcISR(); }
	ISR(USART0_UDRE_vect) { rs485slave.udreISR(); }
	
	void setup() {
		PRR0 &= ~(1<<PRUSART0);
		UBRR0H = 0;
		UBRR0L = 3; // 250000 bps
		UCSR0A = 0;
		UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
		pinMode(TXENABLE_PIN, OUTPUT);
		//UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0) | (1<<TXCIE0);
		rs485slave.clear_txen();
		
	}
	
	void loop() {
		PollingInput::pollInputs();
		ExportStreamListener::loopAll();
	}

}
#endif
