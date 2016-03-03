/* use '#define DCSBIOS_DEFAULT_SERIAL' instead if your Arduino board
 *  does not feature an ATMega328 or ATMega2650 controller.
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

