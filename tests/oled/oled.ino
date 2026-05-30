#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
	Serial.begin(9600);
	delay(1000);
	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Adresse I2C courante 0x3C
		Serial.println(F("Erreur OLED SSD1306 non détecté !"));
		while (1);
	}
	display.clearDisplay();
	display.setTextSize(2);
	display.setTextColor(SSD1306_WHITE);
	display.setCursor(0, 0);
	display.println(F("Hello, OLED!"));
	display.display();
}

void loop() {
	// Rien à faire ici pour le test
}
