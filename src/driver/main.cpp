#include <Arduino.h>

// Arduino Uno pin mapping:

/*
L298N Motor Driver:
│
├── 10: ─────────────────► ENA: (L298N, PWM motor 1)
├── 8: ──────────────────► IN1: (L298N, motor 1 direction)
├── 9: ──────────────────► IN2: (L298N, motor 1 direction)
│
├── 13: ─────────────────► ENB: (L298N, PWM motor 2)
├── 11: ─────────────────► IN3: (L298N, motor 2 direction)
└── 12: ─────────────────► IN4: (L298N, motor 2 direction)

Mode button:
│
├── 4: ──────────────────► AUTO: (mode auto/manual switch button)

Control buttons - work only in manual mode:
│
├── 2: ──────────────────► RETRACT: (retract button)
└── 3: ──────────────────► DEPLOY: (deploy button)

LDR 1 - sun sensor 1:
|
├── A0: ─────────────────► LDR 1: (analog input)

LDR 2 - sun sensor 2:
|
├── A1: ─────────────────► LDR 2: (analog input)

LDR 3 - night sensor:
|
├── A2: ─────────────────► LDR 3: (analog input)
*/

#define TRUE 1
#define FALSE 0

#define PULL_DEBUG FALSE
#define EVENT_DEBUG TRUE

#define LDR_1_PIN A0
#define LDR_2_PIN A1
#define RETRACT_BUTTON_PIN 2
#define DEPLOY_BUTTON_PIN 3

#define AUTO_BUTTON_PIN 4
#define SCAN_BUTTON_PIN 5

#define LDR_THRESHOLD 10

static int eventDebugId = 1;

static struct {
  const int ldr1Pin = LDR_1_PIN;
  const int ldr2Pin = LDR_2_PIN;
  const int deployButtonPin = DEPLOY_BUTTON_PIN;
  const int retractButtonPin = RETRACT_BUTTON_PIN;
  const int autoButtonPin = AUTO_BUTTON_PIN;
  const int scanButtonPin = SCAN_BUTTON_PIN;

} pinMapping;

bool compareWithThreshold(int ldr1, int ldr2, int threshold) {
  return abs(ldr1 - ldr2) <= threshold;
}

bool isAutoMode() {
  return digitalRead(pinMapping.autoButtonPin) == LOW;
}

void pullDebug(int ldr1Raw, int ldr2Raw, int ldr1Percent, int ldr2Percent) {
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
  Serial.print(digitalRead(pinMapping.deployButtonPin) == LOW ? "Pressed" : "Released");
  Serial.print("  Retract: ");
  Serial.print(digitalRead(pinMapping.retractButtonPin) == LOW ? "Pressed" : "Released");
  Serial.print("  Auto: ");
  Serial.print(digitalRead(pinMapping.autoButtonPin) == LOW ? "On" : "Off");
  Serial.print("  Scan: ");
  Serial.print(digitalRead(pinMapping.scanButtonPin) == LOW ? "Pressed" : "Released");
  Serial.println();
  Serial.println("------------------------------");
}


void setupPinDebug() {
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
  Serial.print("SCAN_BUTTON_PIN: ");
  Serial.println(SCAN_BUTTON_PIN);
}

void setupPin() {
  pinMode(LDR_1_PIN, INPUT);
  pinMode(LDR_2_PIN, INPUT);
  pinMode(DEPLOY_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RETRACT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(AUTO_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SCAN_BUTTON_PIN, INPUT_PULLUP);
  if (PULL_DEBUG || EVENT_DEBUG) setupPinDebug();
}

void setup() {
  if (PULL_DEBUG || EVENT_DEBUG) {
    Serial.begin(9600);
    Serial.println("Debug mode enabled");
  }
  setupPin();
}

void loop() {
  int ldr1Raw = analogRead(LDR_1_PIN);
  int ldr2Raw = analogRead(LDR_2_PIN);
  int ldr1Percent = map(ldr1Raw, 0, 1023, 100, 0);
  int ldr2Percent = map(ldr2Raw, 0, 1023, 100, 0);

  if (EVENT_DEBUG) {
    if (!compareWithThreshold(ldr1Percent, ldr2Percent, LDR_THRESHOLD)) {
      Serial.print("[EVT-");
      Serial.print(eventDebugId);
      Serial.print("] LDR values are different with threshold ");
      Serial.print(LDR_THRESHOLD);
      Serial.println("%");
      Serial.print("[EVT-");
      Serial.print(eventDebugId);
      Serial.print("] LDR1: ");
      Serial.print(ldr1Raw);
      Serial.print("  LDR2: ");
      Serial.println(ldr2Raw);
      eventDebugId++;
    }

    if (!isAutoMode()) {
      if (digitalRead(pinMapping.deployButtonPin) == LOW) {
        Serial.print("[EVT-");
        Serial.print(eventDebugId);
        Serial.println("] Deploy button pressed");
        eventDebugId++;
      }
      if (digitalRead(pinMapping.retractButtonPin) == LOW) {
        Serial.print("[EVT-");
        Serial.print(eventDebugId);
        Serial.println("] Retract button pressed");
        eventDebugId++;
      }
    }
    if (digitalRead(pinMapping.scanButtonPin) == LOW) {
      Serial.print("[EVT-");
      Serial.print(eventDebugId);
      Serial.println("] Scan button pressed");
      eventDebugId++;
    }
    if (digitalRead(pinMapping.autoButtonPin) == LOW) {
      Serial.print("[EVT-");
      Serial.print(eventDebugId);
      Serial.println("] Auto button pressed");
      eventDebugId++;
    }
  }
  if (PULL_DEBUG) pullDebug(ldr1Raw, ldr2Raw, ldr1Percent, ldr2Percent);

  delay(500);
}
