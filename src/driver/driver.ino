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
extern const Settings settings;
static SerialPrint serialPrinter;

static LDRs ldrs(
  LDR(settings.pin.LDR.day.up, ANALOG_RESOLUTION),
  LDR(settings.pin.LDR.day.down, ANALOG_RESOLUTION),
  LDR(settings.pin.LDR.night, ANALOG_RESOLUTION)
);

static Motors motors(
  Motor(settings.pin.motors.in1, settings.pin.motors.in2, settings.pin.motors.ena, PWM_RESOLUTION),
  Motor(settings.pin.motors.in3, settings.pin.motors.in4, settings.pin.motors.enb, PWM_RESOLUTION)
);

// -------------------
// Program definitions
// -------------------
void setup();
static void setupPin(SettingsPin pin);
static void setupBlinkStatusLED(uint8_t pin);

void loop();
static void loopAutoMode();
static void loopManualMode();

// -----------------
// Utility functions
// -----------------
static bool isAutoMode() {
  return digitalRead(settings.pin.button.automatic) == LOW;
}

// -----
// Setup
// -----
static void setupPin(SettingsPin pin) {
  pinMode(pin.LDR.day.up, INPUT);
  pinMode(pin.LDR.day.down, INPUT);
  pinMode(pin.LDR.night, INPUT);
  pinMode(pin.button.deploy, INPUT_PULLUP);
  pinMode(pin.button.retract, INPUT_PULLUP);
  pinMode(pin.button.automatic, INPUT_PULLUP);
  pinMode(pin.button.scan, INPUT_PULLUP);
  pinMode(pin.LEDStatus, OUTPUT);
}

static void setupBlinkStatusLED(uint8_t pin) {
  for (int i = 0; i < 3; i++) {
    digitalWrite(pin, HIGH);
    delay(1000);
    digitalWrite(pin, LOW);
    delay(1000);
  }
}

void setup() {
  if (DEBUG) {
    Serial.begin(9600);
    delay(2000); // Let the serial monitor open
    Serial.println("Debug mode enabled");
  }
  setupPin(settings.pin);
  if (DEBUG) serialPrinter.eventln("The program will start...");
  setupBlinkStatusLED(settings.pin.LEDStatus);
  if (DEBUG && isAutoMode()) {
    serialPrinter.eventln("Auto mode active");
  } else if (DEBUG) {
    serialPrinter.eventln("Manual mode active");
  }
}

// -----
// Loops
// -----
static void loopAutoMode() {
  unsigned long now = millis();
  unsigned long lastIterationTime = 0;

  if (now - lastIterationTime <= 1000) {
    ldrs.read();
    bool isLDRDifferent = ldrs.isDayUpDifferentFromDayDown(settings.program.LDR.threshold);

    if (DEBUG && isLDRDifferent) {
      serialPrinter.event("LDR values are different with threshold ");
      Serial.print(settings.program.LDR.threshold);
      Serial.println("%");

      serialPrinter.LDR(ldrs.dayUp, "  LDR day up: ");
      serialPrinter.LDR(ldrs.dayDown, "  LDR day down: ");
    }

    if (isLDRDifferent) {
      if (ldrs.isDayUpBrighterThanDayDown(settings.program.LDR.threshold)) {
        if (DEBUG) serialPrinter.eventln("Deploying motors");
        motors.deploy(settings.program.motor.speed);
      } else if (ldrs.isDayDownBrighterThanDayUp(settings.program.LDR.threshold)) {
        if (DEBUG) serialPrinter.eventln("Retracting motors");
        motors.retract(settings.program.motor.speed);
      }
    }
  }
}

static void loopManualMode() {
  if (digitalRead(settings.pin.button.deploy) == LOW) {
    if (DEBUG) serialPrinter.eventln("Deploy button pressed");
    motors.deploy(settings.program.motor.speed);
  } else if (digitalRead(settings.pin.button.retract) == LOW) {
    if (DEBUG) serialPrinter.eventln("Retract button pressed");
    motors.retract(settings.program.motor.speed);
  } else if (digitalRead(settings.pin.button.scan) == LOW) {
    if (DEBUG) serialPrinter.eventln("Scan button pressed");
    // Implement scan functionality here
  }
}

void loop() {
  if (isAutoMode()) {
    loopAutoMode();
  } else if (DEBUG) {
    serialPrinter.eventln("Switched to manual mode");
    serialPrinter.eventln("Waiting for button presses...");
  }
}

