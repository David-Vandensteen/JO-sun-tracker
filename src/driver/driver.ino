/*
  David Vandensteen
  MIT License
  JO Sun Tracker - Driver code for Arduino Uno/Nano/ESP32 WROOM 32S
  @2026
*/
#include <Arduino.h>
#include "driver.h"
#include "setting.h"
#include "led_protocol.h"
#include "trackers.h"

static Setting setting;
static LedProtocol *ledProtocol;
static Trackers *trackers;

void setup() {
  settingInit(&setting);
  Driver::init(&setting);
}

void loop() {
  Driver::update();
}
