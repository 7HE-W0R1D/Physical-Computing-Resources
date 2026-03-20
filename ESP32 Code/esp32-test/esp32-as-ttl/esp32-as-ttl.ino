void setup() {
  Serial.begin(115200);   // 与电脑通信
  Serial1.begin(115200, SERIAL_8N1, 16, 17); // 与雷达通信 (RX=16, TX=17)
}

void loop() {
  // 从电脑转发到雷达
  if (Serial.available()) {
    Serial1.write(Serial.read());
  }
  // 从雷达转发到电脑
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
}