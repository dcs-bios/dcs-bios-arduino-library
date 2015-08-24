#define DCSBIOS_RS485_MASTER
#define UART1_TXENABLE_PIN 2
#define UART2_TXENABLE_PIN 3
#define UART3_TXENABLE_PIN 4
#include "DcsBios.h"

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, 0);
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}

