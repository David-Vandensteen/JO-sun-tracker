#include <Arduino.h>
#define VERSION "0.0.1"
/*
  David Vandensteen
  MIT License
  JO Sun Tracker - Driver code for Arduino Uno
  @2026
*/

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
#define ANALOG_RESOLUTION 1023
#define PWM_RESOLUTION 255

// ---------------------------
// Program settings
// ---------------------------
#define SETTINGS_PROGRAM_LDR_THRESHOLD 10
#define SETTINGS_PROGRAM_MOTOR_SPEED 30

// -------------------------------
// Global variables and constants
// -------------------------------
static uint16_t eventDebugId = 1;

// ---------------------------------------------
// Settings structures and constants
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

typedef struct SettingsPinMotors {
  const uint8_t ena;
  const uint8_t in1;
  const uint8_t in2;
  const uint8_t enb;
  const uint8_t in3;
  const uint8_t in4;
} SettingsPinMotor;

typedef struct SettingsPin {
  const SettingsPinLDR LDR;
  const SettingsPinMotor motors;
  const SettingsPinButton button;

} SettingsPin;

typedef struct SettingsProgramLDR {
  const uint16_t threshold;
} SettingsProgramLDR;

typedef struct SettingsProgramMotor {
  const int speed;
} SettingsProgramMotor;

typedef struct SettingsProgram {
  const char* version;
  const SettingsProgramLDR LDR;
  const SettingsProgramMotor motor;
} SettingsProgram;


typedef struct Settings {
  const SettingsPin pin;
  const SettingsProgram program;
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
    .motors = {
      .ena = 10,
      .in1 = 8,
      .in2 = 9,
      .enb = 13,
      .in3 = 11,
      .in4 = 12
    },
    .button = {
      .deploy = 2,
      .retract = 3,
      .automatic = 4,
      .scan = 5
    }
  },
  .program = {
    .version = VERSION,
    .LDR = {
      .threshold = SETTINGS_PROGRAM_LDR_THRESHOLD
    },
    .motor = {
      .speed = SETTINGS_PROGRAM_MOTOR_SPEED
    }
  }
};

// ----
// LDR Structures
// ---
typedef struct LDR {
  int raw;
  int percent;
} LDR;

typedef struct LDRs {
  LDR dayUp;
  LDR dayDown;
  LDR night;
} LDRs;

// ---------------------------
// Utility functions
// ---------------------------
bool compareWithThreshold(long first, long second, long threshold) {
  return abs(first - second) <= threshold;
}

bool isAutoMode() {
  return digitalRead(settings.pin.button.automatic) == LOW;
}

// ----------------
// LDR functions
// ---------------
void LDRsRead(LDRs* ldrs) {
  ldrs->dayUp.raw = analogRead(settings.pin.LDR.day.up);
  ldrs->dayDown.raw = analogRead(settings.pin.LDR.day.down);
  ldrs->dayUp.percent = map(ldrs->dayUp.raw, 0, ANALOG_RESOLUTION, 100, 0);
  ldrs->dayDown.percent = map(ldrs->dayDown.raw, 0, ANALOG_RESOLUTION, 100, 0);
  ldrs->night.raw = analogRead(settings.pin.LDR.night);
  ldrs->night.percent = map(ldrs->night.raw, 0, ANALOG_RESOLUTION, 100, 0);
}

// -----------------
// Motor functions
// ----------------------
void MotorDeployByPin(uint8_t inPin1, uint8_t inPin2, uint8_t enPin, int speedPercent) {
  digitalWrite(inPin1, HIGH);
  digitalWrite(inPin2, LOW);
  analogWrite(enPin, map(speedPercent, 0, 100, 0, PWM_RESOLUTION));
}

void MotorDeployById(uint8_t id, int speedPercent) {
  switch (id) {
  case 1:
    MotorDeployByPin(settings.pin.motors.in1, settings.pin.motors.in2, settings.pin.motors.ena, speedPercent);
    break;
  case 2:
    MotorDeployByPin(settings.pin.motors.in3, settings.pin.motors.in4, settings.pin.motors.enb, speedPercent);
    break;

  default:
    break;
  }
}

void MotorsDeploy(int speedPercent) {
  MotorDeployById(1, speedPercent);
  MotorDeployById(2, speedPercent);
}

void MotorRetractByPin(uint8_t inPin1, uint8_t inPin2, uint8_t enPin, int speedPercent) {
  digitalWrite(inPin1, LOW);
  digitalWrite(inPin2, HIGH);
  analogWrite(enPin, map(speedPercent, 0, 100, 0, PWM_RESOLUTION));
}

void MotorRetractById(uint8_t id, int speedPercent) {
  switch (id) {
  case 1:
    MotorRetractByPin(settings.pin.motors.in1, settings.pin.motors.in2, settings.pin.motors.ena, speedPercent);
    break;
  case 2:
    MotorRetractByPin(settings.pin.motors.in3, settings.pin.motors.in4, settings.pin.motors.enb, speedPercent);
    break;

  default:
    break;
  }
}

void MotorsRetract(int speedPercent) {
  MotorRetractById(1, speedPercent);
  MotorRetractById(2, speedPercent);
}

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

void setupSettingsPinDebug() {
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

void setupSettingsProgramDebug() {
  Serial.println("Program settings:");
  Serial.print("  Version: "); Serial.println(settings.program.version);
  Serial.print("  LDR threshold: "); Serial.print(settings.program.LDR.threshold); Serial.println("%");
  Serial.print("  Motor speed: "); Serial.print(settings.program.motor.speed); Serial.println("%");
}

void setupSettingsDebug() {
  setupSettingsPinDebug();
  setupSettingsProgramDebug();
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
  if (PULL_DEBUG || EVENT_DEBUG) setupSettingsDebug();
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
  LDRs ldrs;
  LDRsRead(&ldrs);

  if (EVENT_DEBUG) {
    if (!compareWithThreshold(ldrs.dayUp.percent, ldrs.dayDown.percent, settings.program.LDR.threshold)) {
      Serial.print("[EVT-");
      Serial.print(eventDebugId);
      Serial.print("] LDR values are different with threshold ");
      Serial.print(settings.program.LDR.threshold);
      Serial.println("%");
      Serial.print("[EVT-");
      Serial.print(eventDebugId);
      Serial.print("] LDR1: ");
      Serial.print(ldrs.dayUp.raw);
      Serial.print(" (");
      Serial.print(ldrs.dayUp.percent);
      Serial.print("%)  LDR2: ");
      Serial.print(ldrs.dayDown.raw);
      Serial.print(" (");
      Serial.print(ldrs.dayDown.percent);
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
  if (PULL_DEBUG) pullDebug(ldrs.dayUp.raw, ldrs.dayDown.raw, ldrs.dayUp.percent, ldrs.dayDown.percent);

  delay(500);
}
