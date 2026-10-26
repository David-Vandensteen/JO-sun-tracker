/*
  David Vandensteen
  MIT License
  JO Sun Tracker - Driver code for Arduino Uno/Nano/ESP32
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
  DriverState state = Driver::update();
  if (state.isStop()) {
    Driver::stop();
  }
  if (state.isReset()) {
    Driver::reset();
  }
}
