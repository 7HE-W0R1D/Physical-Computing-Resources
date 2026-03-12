const int ledPin = 22;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Blink test started.");
}

void loop() {
  digitalWrite(ledPin, HIGH);
  Serial.println("LED ON");
  delay(400);

  digitalWrite(ledPin, LOW);
  Serial.println("LED OFF");
  delay(100);
}