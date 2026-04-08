#include <Arduino.h>

#define TRUE 1
#define FALSE 0

#define PULL_DEBUG FALSE
#define EVENT_DEBUG TRUE

#define LDR_1_PIN A0
#define LDR_2_PIN A1
#define RETRACT_BUTTON_PIN 2
#define DEPLOY_BUTTON_PIN 3

#define AUTO_BUTTON_PIN 4

#define LDR_THRESHOLD 10

bool isAutoMode() {
  return digitalRead(AUTO_BUTTON_PIN) == LOW;
}

bool compare_ldr_percent(int ldr1, int ldr2, int threshold) {
  return abs(ldr1 - ldr2) <= threshold;
}

void setup_pin() {
  pinMode(LDR_1_PIN, INPUT);
  pinMode(LDR_2_PIN, INPUT);
  pinMode(DEPLOY_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RETRACT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(AUTO_BUTTON_PIN, INPUT_PULLUP);
  if (PULL_DEBUG || EVENT_DEBUG) {
    Serial.println("Pin setup completed");
    Serial.print("LDR_1_PIN: ");
    Serial.println(LDR_1_PIN);
    Serial.print("LDR_2_PIN: ");
    Serial.println(LDR_2_PIN);
    Serial.print("DEPLOY_BUTTON_PIN: ");
    Serial.println(DEPLOY_BUTTON_PIN);
    Serial.print("RETRACT_BUTTON_PIN: ");
    Serial.println(RETRACT_BUTTON_PIN);
    Serial.print("AUTO_BUTTON_PIN: ");
    Serial.println(AUTO_BUTTON_PIN);
  }
}

void setup() {
  if (PULL_DEBUG || EVENT_DEBUG) {
    Serial.begin(9600);
    Serial.println("Debug mode enabled");
  }
  setup_pin();
}

void loop() {
  int ldr1Raw = analogRead(LDR_1_PIN);
  int ldr2Raw = analogRead(LDR_2_PIN);
  int ldr1Percent = map(ldr1Raw, 0, 1023, 100, 0);
  int ldr2Percent = map(ldr2Raw, 0, 1023, 100, 0);

  if (EVENT_DEBUG) {
    if (!compare_ldr_percent(ldr1Percent, ldr2Percent, LDR_THRESHOLD)) {
      Serial.print("LDR values are different with threshold ");
      Serial.print(LDR_THRESHOLD);
      Serial.println("%");
      Serial.print("LDR1: ");
      Serial.print(ldr1Raw);
      Serial.print("  LDR2: ");
      Serial.println(ldr2Raw);
    }

    if (!isAutoMode()) {
      if (digitalRead(DEPLOY_BUTTON_PIN) == LOW) {
        Serial.println("Deploy button pressed");
      }
      if (digitalRead(RETRACT_BUTTON_PIN) == LOW) {
        Serial.println("Retract button pressed");
      }
    }
    if (digitalRead(AUTO_BUTTON_PIN) == LOW) {
      Serial.println("Auto button pressed");
    }
  }

  if (PULL_DEBUG) {
    Serial.println("------------------------------");
    Serial.print("Lum1: ");
    Serial.print(ldr1Raw);
    Serial.print(" (");
    Serial.print(ldr1Percent);
    Serial.print("%)  Lum2: ");
    Serial.print(ldr2Raw);
    Serial.print(" (");
    Serial.print(ldr2Percent);
    Serial.print("%)");
    Serial.print("  Deploy: ");
    Serial.print(digitalRead(DEPLOY_BUTTON_PIN) == LOW ? "Pressed" : "Released");
    Serial.print("  Retract: ");
    Serial.print(digitalRead(RETRACT_BUTTON_PIN) == LOW ? "Pressed" : "Released");
    Serial.print("  Auto: ");
    Serial.print(digitalRead(AUTO_BUTTON_PIN) == LOW ? "On" : "Off");
    Serial.println("------------------------------");
  }
  delay(500);
}
