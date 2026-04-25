
#include <WiFi.h>
#include <WebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include "secret.h"

WebServer server(80);

void handleRoot() {
  File file = SPIFFS.open("/index.html", "r");
  if (!file || file.isDirectory()) {
    server.send(500, "text/plain", "Erreur: index.html non trouvé");
    return;
  }
  String html = file.readString();
  file.close();
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  delay(5000);
  if (!SPIFFS.begin(true)) {
    Serial.println("Erreur SPIFFS");
    return;
  }
  WiFi.begin(SSID, PASSWORD);
  Serial.print("Connexion au WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connecte, IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("Serveur HTTP demarre");
}

void loop() {
  server.handleClient();
}
