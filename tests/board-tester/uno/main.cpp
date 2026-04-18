#include <Arduino.h>

// List of Arduino Uno digital pins
const int digitalPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
const int nbDigitalPins = sizeof(digitalPins) / sizeof(digitalPins[0]);

void setup() {
	Serial.begin(9600);
	delay(2000); // Let the serial monitor open
	Serial.println("Arduino Uno GPIO test");

	// Test pins as outputs
	for (int i = 0; i < nbDigitalPins; i++) {
		int pin = digitalPins[i];
		pinMode(pin, OUTPUT);
		Serial.print("Output test pin ");
		Serial.print(pin);
		Serial.println(": HIGH");
		digitalWrite(pin, HIGH);
	}

	Serial.println("Test finished.");
}

void loop() {
	// Nothing to do
}
