/*
  The following #define tells DCS-BIOS that this is a RS-485 slave device.
  It also sets the address of this slave device. The slave address should be
  between 1 and 126 and must be unique among all devices on the same bus.
*/
#define DCSBIOS_RS485_SLAVE 1

#include "DcsBios.h"

DcsBios::Switch2Pos lampTestBtn("LAMP_TEST_BTN", 10);
DcsBios::LED nmspIlsLed(0x1112, 0x0020, 13);

void setup() {
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}

