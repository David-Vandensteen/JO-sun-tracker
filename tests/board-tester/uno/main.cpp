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
		Serial.print(": HIGH -> ");
		digitalWrite(pin, HIGH);
		delay(200);
		Serial.print(digitalRead(pin));
		Serial.print(", LOW -> ");
		digitalWrite(pin, LOW);
		delay(200);
		Serial.println(digitalRead(pin));
		pinMode(pin, INPUT);
	}

	// Test pins as inputs (floating read)
	for (int i = 0; i < nbDigitalPins; i++) {
		int pin = digitalPins[i];
		pinMode(pin, INPUT);
		Serial.print("Input test pin ");
		Serial.print(pin);
		Serial.print(": ");
		Serial.println(digitalRead(pin));
	}

	Serial.println("Test finished.");
}

void loop() {
	// Nothing to do
}
