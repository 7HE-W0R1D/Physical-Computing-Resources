#include <Arduino.h>

#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(115200);

  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  Serial.println("Bidirectional serial initialized.");
  Serial.println("Listening on USB and pins 16/17...");
}

void loop() {
  if (Serial2.available()) {
    String incomingFromDevice = Serial2.readStringUntil('\n');

    Serial.print("From Device (Serial2): ");
    Serial.println(incomingFromDevice);
  }

  if (Serial.available()) {
    String incomingFromUSB = Serial.readStringUntil('\n');

    Serial2.print(incomingFromUSB);
    Serial2.print('\n'); 

    Serial.print("Sent to Device: ");
    Serial.println(incomingFromUSB);
  }
}