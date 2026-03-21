#include <Arduino.h>
#include <LD2420.h>

LD2420 radar;

#define RX1_PIN 16
#define TX1_PIN 17

#define LED_PIN 22

unsigned long previousLedMillis = 0;
int blinkInterval = 2000;
bool ledState = false;

unsigned long lastPrintTime = 0;

void setup() {

  Serial.begin(115200);
  delay(1000); 
  
  Serial.println("\n=== LD2420 Distance & LED Control ===");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial1.begin(115200, SERIAL_8N1, RX1_PIN, TX1_PIN);


  if (radar.begin(Serial1)) {
    Serial.println("LD2420 initialized successfully!");
  } else {
    Serial.println("Warning: Failed to initialize or waiting for data...");
  }

  Serial.println("System ready. Move around to test distance and LED speed.");
  Serial.println("----------------------------------------");
}

String stateToString(LD2420_DetectionState state) {
  switch (state) {
    case LD2420_NO_DETECTION:      return "No Detection";
    case LD2420_DETECTION_ACTIVE:  return "Active Detection";
    case LD2420_DETECTION_LOST:    return "Detection Lost";
    default:                       return "Unknown";
  }
}

void loop() {

  radar.update();


  if (millis() - lastPrintTime >= 500) {  
    lastPrintTime = millis();
    
    if (radar.isDataValid()) {
      LD2420_Data data = radar.getCurrentData();
      

      Serial.print("Radar Status -> State: ");
      Serial.print(stateToString(data.state));
      Serial.print(" | Distance: ");
      Serial.print(data.distance);
      Serial.println(" cm");

      int safeDistance = constrain(data.distance, 0, 400);

      blinkInterval = map(safeDistance, 0, 400, 5, 2000);
      
    } else {
      Serial.println("Radar Status -> No valid data yet");
    }
  }


  unsigned long currentLedMillis = millis();
  if (currentLedMillis - previousLedMillis >= blinkInterval) {
    previousLedMillis = currentLedMillis;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}