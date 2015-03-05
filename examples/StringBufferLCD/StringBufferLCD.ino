#include <DcsBios.h>
#include <Servo.h>
#include <LiquidCrystal.h>

/**** Make your changes after this line ****/

// LCD Display
LiquidCrystal lcd(22, 24, 26, 28, 30, 32);

// Huey Radar Alt
DcsBios::StringBuffer<4> raltDisplayBuffer(0x14aa);

/**** In most cases, you do not have to change anything below this line ****/

/* Instantiate a ProtocolParser object to parse the DCS-BIOS export stream */
DcsBios::ProtocolParser parser;

void setup() {
  Serial.begin(250000);
  
  // Set up the LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("ALT:");
}

/*
Your main loop needs to pass data from the DCS-BIOS export
stream to the parser object you instantiated above.

It also needs to call DcsBios::PollingInput::pollInputs()
to detect changes in the state of connected controls and
pass them on to DCS.
*/
void loop() {
  // feed incoming data to the parser
  int data = Serial.read();
  while (data > -1) {
      parser.processChar(data);
      data = Serial.read();
  }
  
  // poll inputs
  DcsBios::PollingInput::pollInputs();
}

/*
You need to define
void sendDcsBiosMessage(const char* msg, const char* arg)
so that the string msg, followed by a space, the string arg
and a newline gets sent to the DCS-BIOS import stream.

In this example we send it to the serial port, so you need to
run socat to read the data from the serial port and send it
over UDP to DCS-BIOS.

If you are using an Ethernet Shield, you would probably want
to send a UDP packet from this subroutine.
*/
void sendDcsBiosMessage(const char* msg, const char* arg) {
  Serial.write(msg);
  Serial.write(' ');
  Serial.write(arg);
  Serial.write('\n');
}

/*
This subroutine gets called at the end of every update frame
from the export stream (you need to define it even if it
does nothing).

Use this to handle object updates which are not covered by the
DcsBios Arduino library (e.g. displays).
*/
void onDcsBiosFrameSync() {
  // Radar Alt
  if(raltDisplayBuffer.isDirty()) {
    lcd.setCursor(4,0);
    lcd.print(raltDisplayBuffer.buffer);
    raltDisplayBuffer.clearDirty();
  }
}
