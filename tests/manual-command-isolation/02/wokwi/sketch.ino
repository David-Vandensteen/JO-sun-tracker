void setup() {
  Serial.begin(115200);
  pinMode(15, OUTPUT);
  pinMode(17, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(17) == LOW) {
    Serial.println("MODE MANU");
  } else {
    Serial.println("MODE AUTO");
  }
  digitalWrite(15, HIGH);
  delay(1000);
  digitalWrite(15, LOW);
  delay(1000);
}
