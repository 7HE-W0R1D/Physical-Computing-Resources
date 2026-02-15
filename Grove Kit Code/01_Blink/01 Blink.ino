//LED Blink
//The LED will turn on for one second and then turn off for one second
int ledPin = 4; // NOTICE here we have 4, rather than 13 which is in the official example.
void setup() {
    pinMode(ledPin, OUTPUT);
}
void loop() {
    digitalWrite(ledPin, HIGH);
    delay(1000);
    digitalWrite(ledPin, LOW);
    delay(1000);
}
