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


// ---------------------------
// Define
// ---------------------------

#define TRUE 1
#define FALSE 0

#define PULL_DEBUG FALSE
#define EVENT_DEBUG TRUE

#define SETTINGS_LDR_THRESHOLD 10

// -------------------------------
// Global variables and constants
// -------------------------------

static int eventDebugId = 1;

// ---------------------------------------------
// Types, structures and constants for settings
// ----------------------------------------------

typedef struct SettingsPinLDRDay {
  const uint8_t up;
  const uint8_t down;
} SettingsPinLDRDay;

typedef uint8_t SettingsPinLDRNight;

typedef struct SettingsPinLDR {
  const SettingsPinLDRDay day;
  const SettingsPinLDRNight night;
} SettingsPinLDR;

typedef struct SettingsPinButton {
  const uint8_t deploy;
  const uint8_t retract;
  const uint8_t automatic;
  const uint8_t scan;

} SettingsPinButton;

typedef struct SettingsPin {
  const SettingsPinLDR LDR;
  const SettingsPinButton button;

} SettingsPin;

typedef struct Settings {
  const SettingsPin pin;
  uint16_t LDRThreshold;
} Settings;

static const Settings settings = {
  .pin = {
    .LDR = {
      .day = {
        .up = A0,
        .down = A1
      },
      .night = A2
    },
    .button = {
      .deploy = 2,
      .retract = 3,
      .automatic = 4,
      .scan = 5
    }
  },
  .LDRThreshold = SETTINGS_LDR_THRESHOLD
};

// ---------------------------------------------
// Structures for LDR readings and calculations
// ---------------------------------------------

typedef struct LDRRaw {
  int dayUp;
  int dayDown;
} LDRRaw;

typedef struct LDRPercent {
  long dayUp;
  long dayDown;
} LDRPercent;

typedef struct LDRValues {
  LDRRaw raw;
  LDRPercent percent;
} LDRValues;


// -----------------------------------
// Debug functions for serial output
// -----------------------------------

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
  Serial.print(digitalRead(settings.pin.button.deploy) == LOW ? "Pressed" : "Released");
  Serial.print("  Retract: ");
  Serial.print(digitalRead(settings.pin.button.retract) == LOW ? "Pressed" : "Released");
  Serial.print("  Auto: ");
  Serial.print(digitalRead(settings.pin.button.automatic) == LOW ? "On" : "Off");
  Serial.print("  Scan: ");
  Serial.print(digitalRead(settings.pin.button.scan) == LOW ? "Pressed" : "Released");
  Serial.println();
  Serial.println("------------------------------");
}

void setupPinDebug() {
  Serial.println("Pin setup completed");
  Serial.println("Settings:");
  Serial.print("  LDR day up: "); Serial.println(settings.pin.LDR.day.up);
  Serial.print("  LDR day down: "); Serial.println(settings.pin.LDR.day.down);
  Serial.print("  LDR night: "); Serial.println(settings.pin.LDR.night);
  Serial.print("  Deploy button: "); Serial.println(settings.pin.button.deploy);
  Serial.print("  Retract button: "); Serial.println(settings.pin.button.retract);
  Serial.print("  Automatic button: "); Serial.println(settings.pin.button.automatic);
  Serial.print("  Scan button: "); Serial.println(settings.pin.button.scan);
}

// ---------------------------
// Utility functions
// ---------------------------

bool compareWithThreshold(long first, long second, long threshold) {
  return abs(first - second) <= threshold;
}

bool isAutoMode() {
  return digitalRead(settings.pin.button.automatic) == LOW;
}

// ---------------------------
// Setup
// ---------------------------

void setupPin() {
  pinMode(settings.pin.LDR.day.up, INPUT);
  pinMode(settings.pin.LDR.day.down, INPUT);
  pinMode(settings.pin.LDR.night, INPUT);
  pinMode(settings.pin.button.deploy, INPUT_PULLUP);
  pinMode(settings.pin.button.retract, INPUT_PULLUP);
  pinMode(settings.pin.button.automatic, INPUT_PULLUP);
  pinMode(settings.pin.button.scan, INPUT_PULLUP);
  if (PULL_DEBUG || EVENT_DEBUG) setupPinDebug();
}

void setup() {
  if (PULL_DEBUG || EVENT_DEBUG) {
    Serial.begin(9600);
    Serial.println("Debug mode enabled");
  }
  setupPin();
}

// ---------------------------
// Main loop
// ---------------------------

void loop() {
  LDRValues ldr;
  ldr.raw.dayUp = analogRead(settings.pin.LDR.day.up);
  ldr.raw.dayDown = analogRead(settings.pin.LDR.day.down);
  ldr.percent.dayUp = map(ldr.raw.dayUp, 0, 1023, 100, 0);
  ldr.percent.dayDown = map(ldr.raw.dayDown, 0, 1023, 100, 0);

  if (EVENT_DEBUG) {
    if (!compareWithThreshold(ldr.percent.dayUp, ldr.percent.dayDown, settings.LDRThreshold)) {
      Serial.print("[EVT-");
      Serial.print(eventDebugId);
      Serial.print("] LDR values are different with threshold ");
      Serial.print(settings.LDRThreshold);
      Serial.println("%");
      Serial.print("[EVT-");
      Serial.print(eventDebugId);
      Serial.print("] LDR1: ");
      Serial.print(ldr.raw.dayUp);
      Serial.print(" (");
      Serial.print(ldr.percent.dayUp);
      Serial.print("%)  LDR2: ");
      Serial.print(ldr.raw.dayDown);
      Serial.print(" (");
      Serial.print(ldr.percent.dayDown);
      Serial.println("%)");
      eventDebugId++;
    }
    if (!isAutoMode()) {
      if (digitalRead(settings.pin.button.deploy) == LOW) {
        Serial.print("[EVT-");
        Serial.print(eventDebugId);
        Serial.println("] Deploy button pressed");
        eventDebugId++;
      }
      if (digitalRead(settings.pin.button.retract) == LOW) {
        Serial.print("[EVT-");
        Serial.print(eventDebugId);
        Serial.println("] Retract button pressed");
        eventDebugId++;
      }
    }
    if (digitalRead(settings.pin.button.scan) == LOW) {
      Serial.print("[EVT-");
      Serial.print(eventDebugId);
      Serial.println("] Scan button pressed");
      eventDebugId++;
    }
    if (digitalRead(settings.pin.button.automatic) == LOW) {
      Serial.print("[EVT-");
      Serial.print(eventDebugId);
      Serial.println("] Auto button pressed");
      eventDebugId++;
    }
  }
  if (PULL_DEBUG) pullDebug(ldr.raw.dayUp, ldr.raw.dayDown, ldr.percent.dayUp, ldr.percent.dayDown);

  delay(500);
}
