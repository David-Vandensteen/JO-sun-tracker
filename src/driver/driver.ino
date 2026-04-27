/*
  David Vandensteen
  MIT License
  JO Sun Tracker - Driver code for Arduino Uno/ESP32 WROOM 32S
  @2026
*/

#include <Arduino.h>
#include "setting.h"
#include "trackers.h"

static Setting setting;
static Trackers trackers(&setting);

void setup() { trackers.init(); }
void loop() { trackers.update(); }
