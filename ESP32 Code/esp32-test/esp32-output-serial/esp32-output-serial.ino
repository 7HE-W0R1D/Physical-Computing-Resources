#include <Arduino.h>

#define RXD2 16
#define TXD2 17

void setup() {
  Serial.begin(115200);

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  Serial.println("Serial port initialized. Listening on pins 16 and 17...");
}

void loop() {
  if (Serial2.available()) {
    String incomingData = Serial2.readStringUntil('\n');

    Serial.print("Received from Serial2: ");
    Serial.println(incomingData);
  }
}