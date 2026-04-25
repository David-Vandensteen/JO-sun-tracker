#include <WiFi.h>
#include "secret.h"

const char* ssid     = SSID;
const char* password = PASSWORD;

void setup() {
	Serial.begin(115200);
	delay(1000);
	Serial.println();
	Serial.print("Connexion au WiFi : ");
	Serial.println(ssid);

	WiFi.begin(ssid, password);

	int retry = 0;
	while (WiFi.status() != WL_CONNECTED && retry < 20) {
		delay(500);
		Serial.print(".");
		retry++;
	}

	if (WiFi.status() == WL_CONNECTED) {
		Serial.println("\nWiFi connecté !");
		Serial.print("Adresse IP : ");
		Serial.println(WiFi.localIP());
	} else {
		Serial.println("\nÉchec de connexion au WiFi.");
	}
}

void loop() {
}
