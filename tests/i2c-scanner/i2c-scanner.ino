#include <Wire.h>

void setup() {
	Wire.begin();
	Serial.begin(9600);
	delay(1000);
	Serial.println("I2C Scanner...");
	for (byte addr = 1; addr < 127; addr++) {
    Serial.print("Scanning address 0x");
    if (addr < 16) Serial.print("0");
    Serial.println(addr, HEX);
		Wire.beginTransmission(addr);
		if (Wire.endTransmission() == 0) {
			Serial.print("I2C device found at 0x");
			if (addr < 16) Serial.print("0");
			Serial.println(addr, HEX);
		}
	}
	Serial.println("Scan terminé.");
}

void loop() {}
