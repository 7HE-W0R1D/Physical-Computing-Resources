// Enable Configuration (Unlock)
//FD FC FB FA 04 00 FF 00 01 00 04 03 02 01

// End Configuration (Lock & Start Detection)
//FD FC FB FA 02 00 FE 00 04 03 02 01

// Restart Module (Software Reset)
//FD FC FB FA 02 00 68 00 04 03 02 01

// Set to Normal Mode (Default: ON/OFF string and distance)
//FD FC FB FA 08 00 12 00 00 00 64 00 00 00 04 03 02 01

// Set to Energy Report Mode (Output energy values for each gate)
//FD FC FB FA 08 00 12 00 00 00 04 00 00 00 04 03 02 01

// Set to Debug Mode (Output RDMAP data)
//FD FC FB FA 08 00 12 00 00 00 00 00 00 00 04 03 02 01


void setup() {
  Serial.begin(115200);   
  Serial1.begin(115200, SERIAL_8N1, 16, 17); //(RX=16, TX=17)
}

void loop() {

  if (Serial.available()) {
    Serial1.write(Serial.read());
  }

  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
}