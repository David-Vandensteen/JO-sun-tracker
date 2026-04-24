/*
  David Vandensteen
  MIT License
  JO Sun Tracker - Driver code for Arduino Uno
  @2026
*/

#include <Arduino.h>
#include "settings.h"
#include "setup.h"
#include "serial_print.h"
#include "logic.h"

// -------------------------------
// Global variables and constants
// -------------------------------
static Settings settings;
static SerialPrint serialPrint;
static Logic logic(&settings);

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

void loop() {
  digitalRead(settings.board.pin.button.automatic) == LOW ? logic.setAutoMode(TRUE) : logic.setAutoMode(FALSE);
  logic.getAutoMode() == TRUE ? logic.runAuto() : logic.runManual();
}

