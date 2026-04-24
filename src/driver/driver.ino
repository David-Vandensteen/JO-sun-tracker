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
#include "logic.h"

// -------------------------------
// Global variables and constants
// -------------------------------
static Settings settings;
static SerialPrint serialPrint;
static bool isAutoMode = TRUE;
static Logic logic(&settings);

// -------------------
// Program definitions
// -------------------
void setup();
void loop();
static void setAutoMode(bool autoMode);

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
  logic.init();
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

void loop() {
  digitalRead(settings.board.pin.button.automatic) == LOW ? setAutoMode(TRUE) : setAutoMode(FALSE);
  isAutoMode == TRUE ? logic.runAuto() : logic.runManual();
}

