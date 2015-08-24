#include <stdint.h>

#define DCSBIOS_RS485_SLAVE 1
#include "DcsBios.h"

DcsBios::Switch2Pos<> lampTestBtn("LAMP_TEST_BTN", 10);
DcsBios::LED<> nmspIlsLed(0x1112, 0x0020, 13);

void setup() {
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}

