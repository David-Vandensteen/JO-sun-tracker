/*
  David Vandensteen
  MIT License
  JO Sun Tracker - Driver code for Arduino Uno/ESP32 WROOM 32S
  @2026
*/

#include <Arduino.h>
#include "settings.h"
#include "trackers.h"

static Settings settings;
static Trackers trackers(&settings);

void setup() { trackers.init(); }
void loop() { trackers.update(); }

