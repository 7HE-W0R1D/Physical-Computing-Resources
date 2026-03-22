#include <Arduino.h>

#define RX1_PIN 16
#define TX1_PIN 17

#define LED_PIN 22

unsigned long previousLedMillis = 0;
int blinkInterval = 2000;
bool ledState = false;

unsigned long lastPrintTime = 0;

int currentDistance = 0;
bool targetDetected = false;

void setup() {
  Serial.begin(115200);
  delay(100); 
  
  Serial.println("\n=== LD2420 Native UART Distance & LED Control ===");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Initialize Radar Hardware Serial
  Serial1.begin(115200, SERIAL_8N1, RX1_PIN, TX1_PIN);
  
  Serial.println("System ready. Move around to test distance and LED speed.");
  Serial.println("----------------------------------------");
}

void loop() {
  // Read incoming ASCII packets continuously from the radar
  while (Serial1.available()) {
    String line = Serial1.readStringUntil('\n');
    line.trim();

    // The LD2420 spits out pure text like "Range 145" when people move
    if (line.startsWith("Range ")) {
      currentDistance = line.substring(6).toInt();
      targetDetected = true;
    } else if (line == "ON") {
      targetDetected = true;
    } else if (line == "OFF") {
      targetDetected = false;
      currentDistance = 0; // Reset distance when target is lost
    }
  }

  // Handle logging and blinking adjustments every 500ms
  if (millis() - lastPrintTime >= 500) {  
    lastPrintTime = millis();
    
    Serial.print("Target Detected: ");
    Serial.print(targetDetected ? "Yes" : "No");
    Serial.print(" | Distance: ");
    Serial.print(currentDistance);
    Serial.println(" cm");

    if (!targetDetected || currentDistance == 0) {
      blinkInterval = 5000; // Special threshold to completely disable LED
    } else {
      int safeDistance = constrain(currentDistance, 0, 400);
      blinkInterval = map(safeDistance, 0, 400, 5, 2000);
    }
  }

  // LED blink logic (fast execution)
  unsigned long currentLedMillis = millis();
  if (currentLedMillis - previousLedMillis >= blinkInterval) {
    previousLedMillis = currentLedMillis;
    if (blinkInterval == 5000) {
      ledState = LOW; // Stay OFF gracefully
    } else {
      ledState = !ledState;
    }
    digitalWrite(LED_PIN, ledState);
  }
}