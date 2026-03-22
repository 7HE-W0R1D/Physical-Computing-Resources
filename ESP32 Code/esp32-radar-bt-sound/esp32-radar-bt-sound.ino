#include <Arduino.h>
#include <LD2420.h>
#include "AudioTools.h"
#include "AudioTools/Communication/A2DPStream.h"
#include "TalkiePCM.h"
#include "Vocab_US_Large.h"
#include "Vocab_US_Gemini.h"

// Radar Configuration
LD2420 radar;
#define RX1_PIN 16
#define TX1_PIN 17
#define LED_PIN 22

// Bluetooth Target Name
const char* name = "Borui Pixel Buds Pro"; 

// AudioTools Configuration
AudioInfo from(8000, 2, 16);  // TTS
AudioInfo to(44100, 2, 16);   // A2DP

A2DPStream a2dp;
FormatConverterStream out(a2dp);
BufferedStream bs(1024, out); 
TalkiePCM voice(bs, from.channels);

unsigned long lastPrintTime = 0;
unsigned long lastVoiceAlert = 0; // Cooldown timer for voice alerts

// ---------- MULTI-THREADING (FreeRTOS) ----------
// Variables shared between Core 0 (Radar/LED) and Core 1 (Bluetooth)
volatile int currentDistance = 0;
volatile LD2420_DetectionState currentState = LD2420_NO_DETECTION;
volatile int currentBlinkInterval = 2000; // Shared LED blink interval

// The background task that runs on Core 0 to prevent UART buffer lag and LED freeze
void radarUpdateTask(void * pvParameters) {
  unsigned long previousLedMillis = 0;
  bool ledState = false;

  for (;;) {
    // 1. Update Radar
    radar.update();
    if (radar.isDataValid()) {
      LD2420_Data data = radar.getCurrentData();
      currentState = data.state;
      currentDistance = data.distance;
    }

    // 2. Handle LED Blinking (Never blocked by voice playback!)
    unsigned long currentLedMillis = millis();
    if (currentLedMillis - previousLedMillis >= currentBlinkInterval) {
      previousLedMillis = currentLedMillis;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }

    // Yield control for 10ms to prevent Watchdog Timeout on Core 0
    vTaskDelay(10 / portTICK_PERIOD_MS); 
  }
}
// -----------------------------------------------

// Helper to dictate digits
void speakDigit(char digit) {
    switch (digit) {
      case '0': voice.say(sp3_ZERO); break;
      case '1': voice.say(sp3_ONE); break;
      case '2': voice.say(sp3_TWO); break;
      case '3': voice.say(sp3_THREE); break;
      case '4': voice.say(sp3_FOUR); break;
      case '5': voice.say(sp3_FIVE); break;
      case '6': voice.say(sp3_SIX); break;
      case '7': voice.say(sp3_SEVEN); break;
      case '8': voice.say(sp3_EIGHT); break;
      case '9': voice.say(sp3_NINE); break;
    }
}

void setup() {
  Serial.begin(115200);
  
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);
  Serial.println("\n=== LD2420 Radar & A2DP Voice System ===");

  // Initialize LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Initialize Radar
  Serial1.begin(115200, SERIAL_8N1, RX1_PIN, TX1_PIN);
  if (radar.begin(Serial1)) {
    Serial.println("LD2420 initialized successfully!");
    
    // Launch Radar Background Task on Core 0
    xTaskCreatePinnedToCore(
      radarUpdateTask,  // Function pointer
      "RadarTask",      // Task name
      4096,             // Stack size
      NULL,             // Parameter
      1,                // Priority
      NULL,             // Task handle
      0                 // Pin to Core 0 (default loop is Core 1)
    );
    Serial.println("Radar & LED background task started on Core 0.");
  } else {
    Serial.println("Warning: Failed to initialize radar...");
  }

  // Initialize Audio Format Converter
  out.begin(from, to);

  // Setup A2DP Stream
  Serial.println("Starting A2DP Source...");
  auto cfg = a2dp.defaultConfig(TX_MODE);
  cfg.name = name;
  cfg.silence_on_nodata = true;  
  cfg.wait_for_connection = false; 
  cfg.auto_reconnect = true; // Use saved MAC address
  a2dp.begin(cfg);
  
  // Wait for Bluetooth connection
  Serial.print("Waiting for auto-reconnection to headphones");
  while (!a2dp.isConnected()) {
     Serial.print(".");
     delay(1000);
  }
  Serial.println("\nBluetooth Connected!");
  
  voice.say(sp_MAN_KOBE); // Announce startup
}

void loop() {
  // Main logic now runs continuously on Core 1
  if (millis() - lastPrintTime >= 500) {  
    lastPrintTime = millis();
    
    // Copy the volatile variables locally so they don't change mid-execution
    int dist = currentDistance;
    LD2420_DetectionState state = currentState;
      
    Serial.print("State: ");
    Serial.print(state);
    Serial.print(" | Distance: ");
    Serial.print(dist);
    Serial.println(" cm");

    int safeDistance = constrain(dist, 0, 400);
    // Update the shared variable so the background task knows how fast to blink
    currentBlinkInterval = map(safeDistance, 0, 400, 5, 2000);

    // Check for human detection & handle voice alert cooldown
    if (state == LD2420_DETECTION_ACTIVE && (millis() - lastVoiceAlert > 5000)) {
       lastVoiceAlert = millis();
       Serial.println("Triggering Voice Alert: DANGER & Distance");
       
       // Speak "DANGER"
       voice.say(sp2_DANGER);
       voice.silence(300);

       // Speak distance digits (e.g., 145 -> "ONE FOUR FIVE")
       String distStr = String(dist);
       for (int i = 0; i < distStr.length(); i++) {
          speakDigit(distStr[i]);
          voice.silence(150);
       }
    }
  }
}
