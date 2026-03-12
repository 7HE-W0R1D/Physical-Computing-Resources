#include <Arduino.h>
#include <LD2420.h>

// Initialize the LD2420 radar object
LD2420 radar;

// Pins for LD2420 communication with ESP32 Serial1
// Adjust these to match your actual wiring if needed.
#define RX1_PIN 16
#define TX1_PIN 17

// Target Pin logic level testing
#define TEST_PIN 33

void setup() {
  // 1. Initialize USB Serial Monitor for debugging
  Serial.begin(115200);
  delay(1000); // Wait for Serial Monitor to connect
  
  Serial.println("\n=== Minimal LD2420 Test Program ===");

  // 2. Initialize the TEST_PIN
  pinMode(TEST_PIN, INPUT);

  // 3. Initialize Serial1 for LD2420 communication
  // 115200 is typical for newer firmware, 256000 for older
  Serial1.begin(115200, SERIAL_8N1, RX1_PIN, TX1_PIN);

  // 4. Start the radar
  if (radar.begin(Serial1)) {
    Serial.println("✓ LD2420 initialized successfully via cyrixninja library!");
  } else {
    // It's possible begin() is non-blocking and returns true easily, 
    // but this gives a standard check.
    Serial.println("✗ Warning: Failed to initialize or waiting for data...");
  }

  Serial.println("Reading data... Move around to see position and pin 33 state.");
  Serial.println("----------------------------------------");
}

unsigned long lastPrintTime = 0;

// Helper to convert state to string
String stateToString(LD2420_DetectionState state) {
  switch (state) {
    case LD2420_NO_DETECTION:      return "No Detection";
    case LD2420_DETECTION_ACTIVE:  return "Active Detection";
    case LD2420_DETECTION_LOST:    return "Detection Lost";
    default:                       return "Unknown";
  }
}

void loop() {
  // 1. Give cycles to the radar library to process incoming serial data
  radar.update();

  // 2. Continually check if we have data to print
  if (millis() - lastPrintTime >= 500) {  // Output every 500ms
    lastPrintTime = millis();
    
    // Read the logic level of pin 33
    int pinState = digitalRead(TEST_PIN);
    
    Serial.print("[Pin 33: ");
    Serial.print(pinState == HIGH ? "HIGH" : "LOW ");
    Serial.print("] | ");

    // Check radar data
    if (radar.isDataValid()) {
      LD2420_Data data = radar.getCurrentData();
      
      Serial.print("Radar Status -> State: ");
      Serial.print(stateToString(data.state));
      Serial.print(" | Distance: ");
      Serial.print(data.distance);
      Serial.println(" cm");
    } else {
      Serial.println("Radar Status -> No valid data yet (still waiting or unconnected)");
    }
  }
}
