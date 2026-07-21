void setup() {
  Serial.begin(115200);
  pinMode(15, OUTPUT);
  pinMode(17, INPUT_PULLUP);
  pinMode(32, OUTPUT);
}

void loop() {
  if (digitalRead(17) == LOW) {
    Serial.println("MODE MANU");
    digitalWrite(32, LOW);
  } else {
    Serial.println("MODE AUTO");
    digitalWrite(32, HIGH);
  }
  digitalWrite(15, HIGH);
  delay(1000);
  digitalWrite(15, LOW);
  delay(1000);
}
