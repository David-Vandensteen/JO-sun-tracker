/*
  David Vandensteen
  MIT License
  JO Sun Tracker - Driver code for ESP32
  @2026
*/
#include <Arduino.h>
#include "driver.h"
#include "driver_state.h"
#include "setting.h"

static Setting *setting;

void setup() {
  setting = settingInit();
  Driver::init(setting);
}

void loop() {
  Driver::update();
  DriverState state = Driver::state();
  if (state.isReset()) {
    Driver::reset();
  }
}
