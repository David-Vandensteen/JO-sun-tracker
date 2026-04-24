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
static bool isAutoMode = TRUE;

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
static void autoMode();
static void manualMode();
static void setAutoMode(bool autoMode);
static void deploy();
static void retract();
static void scan();

// -----
// Setup
// -----
void setup() {
  settingsInit(&settings);
  if (DEBUG) {
    Serial.begin(9600); //TODO make baud rate configurable
    delay(2000); // Let the serial monitor open
    Serial.println("Debug mode enabled");
  }
  Setup::pin(settings.board.pin);
  if (DEBUG) serialPrint.eventln("The program will start...");
  Setup::blinkStatusLED(settings.board.pin.ledStatus);
}

// ------
// Utility functions
// --
static void setAutoMode(bool autoMode) {
  isAutoMode = autoMode;
  if (DEBUG) {
    if (isAutoMode) {
      serialPrint.eventln("Switched to automatic mode");
    } else {
      serialPrint.eventln("Switched to manual mode");
      serialPrint.eventln("Waiting for button presses...");
    }
  }
}

static void deploy() {
  if (DEBUG) serialPrint.eventln("Deploying motors");
  motors.deploy(settings.program.motor.speed);
}

static void retract() {
  if (DEBUG) serialPrint.eventln("Retracting motors");
  motors.retract(settings.program.motor.speed);
}

static void scan() {
  if (DEBUG) serialPrint.eventln("Scan button pressed");
  // Implement scan functionality here
}

static void autoMode() {
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

static void manualMode() {
  uint8_t btn = 0;
  if (digitalRead(settings.board.pin.button.deploy) == LOW) btn = 1;
  else if (digitalRead(settings.board.pin.button.retract) == LOW) btn = 2;
  else if (digitalRead(settings.board.pin.button.scan) == LOW) btn = 3;

  switch (btn) {
    case 1: deploy(); break;
    case 2: retract(); break;
    case 3: scan(); break;
    default: break;
  }
}

void loop() {
  digitalRead(settings.board.pin.button.automatic) == LOW ? setAutoMode(TRUE) : setAutoMode(FALSE);
  isAutoMode == TRUE ? autoMode() : manualMode();
}

