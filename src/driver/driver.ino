#include <Arduino.h>
#include "settings.h"
#include "LDR.h"
#include "motor.h"
#include "serial_print.h"
/*
  David Vandensteen
  MIT License
  JO Sun Tracker - Driver code for Arduino Uno
  @2026
*/

// -------
// Define
// -------
#define TRUE 1
#define FALSE 0

#define DEBUG TRUE

// -------------------------------
// Global variables and constants
// -------------------------------
static uint16_t eventId = 1;
extern const Settings settings;

// -------------------
// Program definitions
// -------------------
void setup();
static void setupPin();

void loop();
static void loopAutoMode();
static void loopManualMode();

// -----------------
// Utility functions
// -----------------
static bool compareWithThreshold(long first, long second, long threshold) {
  return abs(first - second) <= threshold;
}

static bool isAutoMode() {
  return digitalRead(settings.pin.button.automatic) == LOW;
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

// ----------------
// Serial functions
// ----------------
static void serialPrintEvent(const char *message, uint16_t &eventId) {
  Serial.print("[EVT-");
  Serial.print(eventId);
  Serial.print("] ");
  Serial.print(message);
  eventId++;
}

static void serialPrintLDR(LDR ldr, const char *name) {
    Serial.print(name);
    Serial.print(ldr.raw);
    Serial.print(" (");
    Serial.print(ldr.percent);
    Serial.println("%)");
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
      serialPrintEvent("LDR values are different with threshold ", eventId);
      Serial.print(settings.program.LDR.threshold);
      Serial.println("%");

      serialPrintLDR(ldrs.dayUp, "  LDR day up: ");
      serialPrintLDR(ldrs.dayDown, "  LDR day down: ");
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

