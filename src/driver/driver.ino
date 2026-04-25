/*
  David Vandensteen
  MIT License
  JO Sun Tracker - Driver code for Arduino Uno/ESP32 WROOM 32S
  @2026
*/

#include <Arduino.h>
#include "settings.h"
#include "logic.h"

static Settings settings;
static Logic logic(&settings);

void setup() { logic.init(); }

void loop() {
  digitalRead(settings.board.pin.button.automatic) == LOW
    ? logic.setAutoMode(TRUE) : logic.setAutoMode(FALSE);

  logic.getAutoMode() == TRUE ? logic.runAuto() : logic.runManual();
}

