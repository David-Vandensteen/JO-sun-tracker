/*
  David Vandensteen
  MIT License
  JO Sun Tracker - Driver code for Arduino Uno
  @2026
*/

#include <Arduino.h>
#include "settings.h"
#include "setup.h"
#include "ldr.h"
#include "motor.h"
#include "serial_print.h"

// -------
// Define
// -------
#define TRUE 1
#define FALSE 0
#define DEBUG TRUE

// -------------------------------
// Global variables and constants
// -------------------------------
static Settings settings;
static SerialPrint serialPrint;

static LDRs ldrs(
  LDR(settings.board.pin.ldr.day.up, settings.board.adc.resolution),
  LDR(settings.board.pin.ldr.day.down, settings.board.adc.resolution),
  LDR(settings.board.pin.ldr.night, settings.board.adc.resolution)
);

static Motors motors(
  Motor(settings.board.pin.motors.in1, settings.board.pin.motors.in2, settings.board.pin.motors.ena, settings.board.pwm.resolution),
  Motor(settings.board.pin.motors.in3, settings.board.pin.motors.in4, settings.board.pin.motors.enb, settings.board.pwm.resolution)
);

// -------------------
// Program definitions
// -------------------
void setup();
void loop();
static void loopAutoMode();
static void loopManualMode();

// -----------------
// Utility functions
// -----------------
static bool isAutoMode() {
  return digitalRead(settings.board.pin.button.automatic) == LOW;
}

// -----
// Setup
// -----
void setup() {
  settingsInit(&settings);
  if (DEBUG) {
    Serial.begin(9600);
    delay(2000); // Let the serial monitor open
    Serial.println("Debug mode enabled");
  }
  Setup::pin(settings.board.pin);
  if (DEBUG) serialPrint.eventln("The program will start...");
  Setup::blinkStatusLED(settings.board.pin.ledStatus);
  if (DEBUG && isAutoMode()) {
    serialPrint.eventln("Auto mode active");
  } else if (DEBUG) {
    serialPrint.eventln("Manual mode active");
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
    bool isLDRDifferent = ldrs.isDayUpDifferentFromDayDown(settings.program.ldr.threshold);

    if (DEBUG && isLDRDifferent) {
      serialPrint.event("LDR values are different with threshold ");
      Serial.print(settings.program.ldr.threshold);
      Serial.println("%");

      SerialPrint::ldr(&ldrs.dayUp, "  LDR day up: ");
      SerialPrint::ldr(&ldrs.dayDown, "  LDR day down: ");
    }

    if (isLDRDifferent) {
      if (ldrs.isDayUpBrighterThanDayDown(settings.program.ldr.threshold)) {
        if (DEBUG) serialPrint.eventln("Deploying motors");
        motors.deploy(settings.program.motor.speed);
      } else if (ldrs.isDayDownBrighterThanDayUp(settings.program.ldr.threshold)) {
        if (DEBUG) serialPrint.eventln("Retracting motors");
        motors.retract(settings.program.motor.speed);
      }
    }
  }
}

static void loopManualMode() {
  if (digitalRead(settings.board.pin.button.deploy) == LOW) {
    if (DEBUG) serialPrint.eventln("Deploy button pressed");
    motors.deploy(settings.program.motor.speed);
  } else if (digitalRead(settings.board.pin.button.retract) == LOW) {
    if (DEBUG) serialPrint.eventln("Retract button pressed");
    motors.retract(settings.program.motor.speed);
  } else if (digitalRead(settings.board.pin.button.scan) == LOW) {
    if (DEBUG) serialPrint.eventln("Scan button pressed");
    // Implement scan functionality here
  }
}

void loop() {
  if (isAutoMode()) {
    loopAutoMode();
  } else if (DEBUG) {
    serialPrint.eventln("Switched to manual mode");
    serialPrint.eventln("Waiting for button presses...");
  }
}

