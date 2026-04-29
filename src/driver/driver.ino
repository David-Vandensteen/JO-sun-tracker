/*
  David Vandensteen
  MIT License
  JO Sun Tracker - Driver code for Arduino Uno/Nano/ESP32 WROOM 32S
  @2026
*/

#include <Arduino.h>
#include "driver.h"
#include "setting.h"
#include "trackers.h"

static Setting setting;
static Trackers trackers(&setting);

void setup() {
  settingInit(&setting);
  if (LOG) {
    Driver::serial(setting.board.serial.baudRate, LOG_LEVEL);
    Driver::log(setting.program.version);
  }
  trackers.init();
}

void loop() {
  Driver::watchDog();
  trackers.update();
}
