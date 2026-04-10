#include <Arduino.h>

// Arduino Uno pin mapping:
//
// L298N Motor Driver
// │
// ├── 10 ─────────────────► ENA (L298N, PWM motor 1)
// ├── 8 ──────────────────► IN1 (L298N, motor 1 direction)
// ├── 9 ──────────────────► IN2 (L298N, motor 1 direction)
// │
// ├── 13 ─────────────────► ENB (L298N, PWM motor 2)
// ├── 11 ─────────────────► IN3 (L298N, motor 2 direction)
// └── 12 ─────────────────► IN4 (L298N, motor 2 direction)

#define TRANSITION_DELAY 10000

// Broches pour le vérin 1 (canal A)
const int enaPin = 10;  // ENA (PWM)
const int in1Pin = 8;   // IN1
const int in2Pin = 9;   // IN2

// Broches pour le vérin 2 (canal B)
const int enbPin = 13;  // ENB (PWM)
const int in3Pin = 11;  // IN3
const int in4Pin = 12;  // IN4

void setup() {
  Serial.begin(9600);
  Serial.println("L298N Motor Driver Test");
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);
}

void loop() {
  // Déploiement du vérin 1 à 50% de vitesse
  Serial.println("Déploiement du vérin 1 à 50% de vitesse");
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);
  analogWrite(enaPin, 128); // 50% de vitesse
  delay(TRANSITION_DELAY);

  // Rétraction du vérin 2 à 80% de vitesse
  Serial.println("Rétraction du vérin 2 à 80% de vitesse");
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, HIGH);
  analogWrite(enbPin, 204); // 80% de vitesse
  delay(TRANSITION_DELAY);

  // Arrêt des deux vérins
  Serial.println("Arrêt des deux vérins");
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  analogWrite(enaPin, 0); // Arrêt complet
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, LOW);
  analogWrite(enbPin, 0); // Arrêt complet
  delay(TRANSITION_DELAY);

  // Rétraction du vérin 1 à 30% de vitesse
  Serial.println("Rétraction du vérin 1 à 30% de vitesse");
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, HIGH);
  analogWrite(enaPin, 77); // 30% de vitesse
  delay(TRANSITION_DELAY);

  // Déploiement du vérin 2 à 100% de vitesse
  Serial.println("Déploiement du vérin 2 à 100% de vitesse");
  digitalWrite(in3Pin, HIGH);
  digitalWrite(in4Pin, LOW);
  analogWrite(enbPin, 255); // 100% de vitesse
  delay(TRANSITION_DELAY);

  // Arrêt des deux vérins
  Serial.println("Arrêt des deux vérins");
  digitalWrite(in1Pin, LOW);
  digitalWrite(in2Pin, LOW);
  analogWrite(enaPin, 0);
  digitalWrite(in3Pin, LOW);
  digitalWrite(in4Pin, LOW);
  analogWrite(enbPin, 0);
  delay(TRANSITION_DELAY);
}