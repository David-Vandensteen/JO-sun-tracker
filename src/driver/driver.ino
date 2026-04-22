#include <Arduino.h>
#include "settings.h"
#include "setup.h"
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
static SerialPrint serialPrint;

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
void setup() {
  if (DEBUG) {
    Serial.begin(9600);
    delay(2000); // Let the serial monitor open
    Serial.println("Debug mode enabled");
  }
  Setup::pin(settings.pin);
  if (DEBUG) serialPrint.eventln("The program will start...");
  Setup::blinkStatusLED(settings.pin.LEDStatus);
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
    bool isLDRDifferent = ldrs.isDayUpDifferentFromDayDown(settings.program.LDR.threshold);

    if (DEBUG && isLDRDifferent) {
      serialPrint.event("LDR values are different with threshold ");
      Serial.print(settings.program.LDR.threshold);
      Serial.println("%");

      SerialPrint::ldr(&ldrs.dayUp, "  LDR day up: ");
      SerialPrint::ldr(&ldrs.dayDown, "  LDR day down: ");
    }

    if (isLDRDifferent) {
      if (ldrs.isDayUpBrighterThanDayDown(settings.program.LDR.threshold)) {
        if (DEBUG) serialPrint.eventln("Deploying motors");
        motors.deploy(settings.program.motor.speed);
      } else if (ldrs.isDayDownBrighterThanDayUp(settings.program.LDR.threshold)) {
        if (DEBUG) serialPrint.eventln("Retracting motors");
        motors.retract(settings.program.motor.speed);
      }
    }
  }
}

static void loopManualMode() {
  if (digitalRead(settings.pin.button.deploy) == LOW) {
    if (DEBUG) serialPrint.eventln("Deploy button pressed");
    motors.deploy(settings.program.motor.speed);
  } else if (digitalRead(settings.pin.button.retract) == LOW) {
    if (DEBUG) serialPrint.eventln("Retract button pressed");
    motors.retract(settings.program.motor.speed);
  } else if (digitalRead(settings.pin.button.scan) == LOW) {
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

