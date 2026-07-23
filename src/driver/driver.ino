/*
  David Vandensteen
  MIT License
  JO Sun Tracker - Driver code for Arduino Uno/Nano/ESP32
  @2026
*/
#include <Arduino.h>
#include "driver.h"
#include "setting.h"

static Setting setting;

void setup() {
  settingInit(&setting);
  Driver::init(&setting);
}

void loop() {
  Driver::update();
}
