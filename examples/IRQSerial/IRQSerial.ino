/*
  Tell DCS-BIOS to use a serial connection and use interrupt-driven
  communication. The main program will be interrupted to prioritize
  processing incoming data.
  
  This should work on any Arduino that has an ATMega328 controller
  (Uno, Pro Mini, many others).
 */
#define DCSBIOS_IRQ_SERIAL

#include "DcsBios.h"

DcsBios::Switch2Pos lampTestBtn("LAMP_TEST_BTN", 10);
DcsBios::LED nmspIlsLed(0x1112, 0x0020, 13);

void setup() {
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}

