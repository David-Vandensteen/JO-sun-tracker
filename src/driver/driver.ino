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
SerialPrint serialPrinter;
extern const Settings settings;

Motors motors(
  Motor(settings.pin.motors.in1, settings.pin.motors.in2, settings.pin.motors.ena),
  Motor(settings.pin.motors.in3, settings.pin.motors.in4, settings.pin.motors.enb)
);

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
  if (DEBUG) serialPrinter.settings(settings);
}

void setup() {
  if (DEBUG) {
    Serial.begin(9600);
    delay(2000); // Let the serial monitor open
    Serial.println("Debug mode enabled");
  }
  setupPin();
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
  LDRs ldrs;
  ldrs.read(
    settings.pin.LDR.day.up,
    settings.pin.LDR.day.down,
    settings.pin.LDR.night,
    ANALOG_RESOLUTION
  );

  if (DEBUG) {
    if (!compareWithThreshold(ldrs.dayUp.percent, ldrs.dayDown.percent, settings.program.LDR.threshold)) {
      serialPrinter.event("LDR values are different with threshold ");
      Serial.print(settings.program.LDR.threshold);
      Serial.println("%");

      serialPrinter.LDR(ldrs.dayUp, "  LDR day up: ");
      serialPrinter.LDR(ldrs.dayDown, "  LDR day down: ");
    }
  }
  delay(500);
}

static void loopManualMode() {
  Motor motor1(
    settings.pin.motors.in1,
    settings.pin.motors.in2,
    settings.pin.motors.ena
  );
  Motor motor2(
    settings.pin.motors.in3,
    settings.pin.motors.in4,
    settings.pin.motors.enb
  );

  Motors motors(motor1, motor2);

  if (digitalRead(settings.pin.button.deploy) == LOW) {
    if (DEBUG) serialPrinter.eventln("Deploy button pressed");
    motors.deploy(settings.program.motor.speed, PWM_RESOLUTION);
  } else if (digitalRead(settings.pin.button.retract) == LOW) {
    if (DEBUG) serialPrinter.eventln("Retract button pressed");
    motors.retract(settings.program.motor.speed, PWM_RESOLUTION);
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

