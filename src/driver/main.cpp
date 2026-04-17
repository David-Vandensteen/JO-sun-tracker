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
├── 10: ────────► ENA: (L298N, PWM motor 1)
├── 8: ─────────► IN1: (L298N, motor 1 direction)
├── 9: ─────────► IN2: (L298N, motor 1 direction)
│
├── 13: ─────────► ENB: (L298N, PWM motor 2)
├── 11: ─────────► IN3: (L298N, motor 2 direction)
└── 12: ─────────► IN4: (L298N, motor 2 direction)

Mode button:
│
├── 4: ─────────► AUTO: (mode auto/manual switch button)

Control buttons - work only in manual mode:
│
├── 2: ─────────► RETRACT: (retract button)
└── 3: ─────────► DEPLOY: (deploy button)

LDR 1 - sun sensor 1:
|
├── A0: ─────────► LDR 1: (analog input)

LDR 2 - sun sensor 2:
|
├── A1: ─────────► LDR 2: (analog input)

LDR 3 - night sensor:
|
├── A2: ─────────► LDR 3: (analog input)
*/

// -------
// Define
// -------
#define TRUE 1
#define FALSE 0

#define DEBUG TRUE

#define ANALOG_RESOLUTION 1023
#define PWM_RESOLUTION 255

// ----------------
// Program settings
// ----------------
#define SETTINGS_PROGRAM_LDR_THRESHOLD 10
#define SETTINGS_PROGRAM_MOTOR_SPEED 30

// -------------------------------
// Global variables and constants
// -------------------------------
static uint16_t eventId = 1;

// ---------------------------------
// Settings definition and constants
// ---------------------------------
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

// ---------------
// LDR definitions
// ---------------
typedef struct LDR {
  int raw;
  int percent;
} LDR;

typedef struct LDRs {
  LDR dayUp;
  LDR dayDown;
  LDR night;
} LDRs;

static void LDRReadByPin(LDR* ldr, uint8_t pin, int analogResolution);
static void LDRsRead(LDRs* ldrs, int analogResolution);

// ----------------
// Motor definitions
// ----------------
static void MotorDeployByPin(uint8_t inPin1, uint8_t inPin2, uint8_t enPin, int speedPercent, int pwmResolution);
static void MotorDeployById(uint8_t id, int speedPercent, int pwmResolution);
static void MotorsDeploy(int speedPercent, int pwmResolution);
static void MotorRetractByPin(uint8_t inPin1, uint8_t inPin2, uint8_t enPin, int speedPercent, int pwmResolution);
static void MotorRetractById(uint8_t id, int speedPercent, int pwmResolution);
static void MotorsRetract(int speedPercent, int pwmResolution);

// ------------------
// Serial definitions
// ------------------
static void serialPrintSettingsPin(SettingsPin pin);
static void serialPrintSettingsProgram(SettingsProgram program);
static void serialPrintSettings(Settings settings);

// -----------------
// Utility functions
// -----------------
static bool compareWithThreshold(long first, long second, long threshold) {
  return abs(first - second) <= threshold;
}

static bool isAutoMode() {
  return digitalRead(settings.pin.button.automatic) == LOW;
}

// -------------
// LDR functions
// -------------
static void LDRReadByPin(LDR* ldr, uint8_t pin, int analogResolution) {
  ldr->raw = analogRead(pin);
  ldr->percent = map(ldr->raw, 0, analogResolution, 100, 0);
}

static void LDRsRead(LDRs* ldrs, int analogResolution) {
  LDRReadByPin(&ldrs->dayUp, settings.pin.LDR.day.up, analogResolution);
  LDRReadByPin(&ldrs->dayDown, settings.pin.LDR.day.down, analogResolution);
  LDRReadByPin(&ldrs->night, settings.pin.LDR.night, analogResolution);
}

// ---------------
// Motor functions
// ---------------
static void MotorDeployByPin(uint8_t inPin1, uint8_t inPin2, uint8_t enPin, int speedPercent, int pwmResolution) {
  digitalWrite(inPin1, HIGH);
  digitalWrite(inPin2, LOW);
  analogWrite(enPin, map(speedPercent, 0, 100, 0, pwmResolution));
}

static void MotorDeployById(uint8_t id, int speedPercent, int pwmResolution) {
  switch (id) {
  case 1:
    MotorDeployByPin(settings.pin.motors.in1, settings.pin.motors.in2, settings.pin.motors.ena, speedPercent, pwmResolution);
    break;
  case 2:
    MotorDeployByPin(settings.pin.motors.in3, settings.pin.motors.in4, settings.pin.motors.enb, speedPercent, pwmResolution);
    break;

  default:
    break;
  }
}

static void MotorsDeploy(int speedPercent, int pwmResolution) {
  MotorDeployById(1, speedPercent, pwmResolution);
  MotorDeployById(2, speedPercent, pwmResolution);
}

static void MotorRetractByPin(uint8_t inPin1, uint8_t inPin2, uint8_t enPin, int speedPercent, int pwmResolution) {
  digitalWrite(inPin1, LOW);
  digitalWrite(inPin2, HIGH);
  analogWrite(enPin, map(speedPercent, 0, 100, 0, pwmResolution));
}

static void MotorRetractById(uint8_t id, int speedPercent, int pwmResolution) {
  switch (id) {
  case 1:
    MotorRetractByPin(settings.pin.motors.in1, settings.pin.motors.in2, settings.pin.motors.ena, speedPercent, pwmResolution);
    break;
  case 2:
    MotorRetractByPin(settings.pin.motors.in3, settings.pin.motors.in4, settings.pin.motors.enb, speedPercent, pwmResolution);
    break;

  default:
    break;
  }
}

static void MotorsRetract(int speedPercent, int pwmResolution) {
  MotorRetractById(1, speedPercent, pwmResolution);
  MotorRetractById(2, speedPercent, pwmResolution);
}

// ------
// Serial
// ------
static void serialPrintEvent(const char *message, uint16_t &eventId) {
  Serial.print("[EVT-");
  Serial.print(eventId);
  Serial.print("] ");
  Serial.print(message);
  eventId++;
}

static void serialPrintlnEvent(const char *message, uint16_t &eventId) {
  serialPrintEvent(message, eventId);
  Serial.println();
}

static void serialPrintSettingsPin(SettingsPin pin) {
  Serial.println("Pin setup completed");
  Serial.println("Settings:");
  Serial.print("  LDR day up: "); Serial.println(pin.LDR.day.up);
  Serial.print("  LDR day down: "); Serial.println(pin.LDR.day.down);
  Serial.print("  LDR night: "); Serial.println(pin.LDR.night);
  Serial.print("  Deploy button: "); Serial.println(pin.button.deploy);
  Serial.print("  Retract button: "); Serial.println(pin.button.retract);
  Serial.print("  Automatic button: "); Serial.println(pin.button.automatic);
  Serial.print("  Scan button: "); Serial.println(pin.button.scan);
}

static void serialPrintSettingsProgram(SettingsProgram program) {
  Serial.println("Program settings:");
  Serial.print("  Version: "); Serial.println(program.version);
  Serial.print("  LDR threshold: "); Serial.print(program.LDR.threshold); Serial.println("%");
  Serial.print("  Motor speed: "); Serial.print(program.motor.speed); Serial.println("%");
}

static void serialPrintSettings(Settings settings) {
  serialPrintSettingsPin(settings.pin);
  serialPrintSettingsProgram(settings.program);
}

// -----
// Setup
// -----
static void setupPin() {
  pinMode(settings.pin.LDR.day.up, INPUT);
  pinMode(settings.pin.LDR.day.down, INPUT);
  pinMode(settings.pin.LDR.night, INPUT);
  pinMode(settings.pin.button.deploy, INPUT_PULLUP);
  pinMode(settings.pin.button.retract, INPUT_PULLUP);
  pinMode(settings.pin.button.automatic, INPUT_PULLUP);
  pinMode(settings.pin.button.scan, INPUT_PULLUP);
  if (DEBUG) serialPrintSettings(settings);
}

void setup() {
  if (DEBUG) {
    Serial.begin(9600);
    delay(2000); // Let the serial monitor open
    Serial.println("Debug mode enabled");
  }
  setupPin();
  if (DEBUG && isAutoMode()) {
    serialPrintlnEvent("Auto mode active", eventId);
  } else if (DEBUG) {
    serialPrintlnEvent("Manual mode active", eventId);
  }

}

// -----
// Loops
// -----
static void loopAutoMode() {
  LDRs ldrs;
  LDRsRead(&ldrs, ANALOG_RESOLUTION);

  if (DEBUG) {
    if (!compareWithThreshold(ldrs.dayUp.percent, ldrs.dayDown.percent, settings.program.LDR.threshold)) {
      serialPrintEvent("LDR values are different with threshold", eventId);
      Serial.print(settings.program.LDR.threshold);
      Serial.println("%");

      char ldrMsg[80];
      snprintf(ldrMsg, sizeof(ldrMsg), "LDR1: %d (%d%%)  LDR2: %d (%d%%)",
        ldrs.dayUp.raw, ldrs.dayUp.percent, ldrs.dayDown.raw, ldrs.dayDown.percent);
      serialPrintlnEvent(ldrMsg, eventId);
    }
  }
  delay(500);
}

static void loopManualMode() {
  if (digitalRead(settings.pin.button.deploy) == LOW) {
    if (DEBUG) serialPrintlnEvent("Deploy button pressed", eventId);
    MotorsDeploy(settings.program.motor.speed, PWM_RESOLUTION);
  } else if (digitalRead(settings.pin.button.retract) == LOW) {
    if (DEBUG) serialPrintlnEvent("Retract button pressed", eventId);
    MotorsRetract(settings.program.motor.speed, PWM_RESOLUTION);
  } else if (digitalRead(settings.pin.button.scan) == LOW) {
    if (DEBUG) serialPrintlnEvent("Scan button pressed", eventId);
    // Implement scan functionality here
  }
}

void loop() {
  if (isAutoMode()) {
    loopAutoMode();
  } else if (DEBUG) {
    serialPrintlnEvent("Switched to manual mode", eventId);
    serialPrintlnEvent("Waiting for button presses...", eventId);
  }
}

