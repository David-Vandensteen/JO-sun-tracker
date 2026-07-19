void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(15, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(15, HIGH);  // change state of the LED by setting the pin to the HIGH voltage level
  delay(1000);                      // wait for a second
  digitalWrite(15, LOW);   // change state of the LED by setting the pin to the LOW voltage level
  delay(1000);                      // wait for a second
}
