#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "driver.h"
#include "led_protocol.h"
#include "command.h"
#include "trackers.h"

Setting *Driver::_setting;
LedProtocol *Driver::_ledProtocol;
Trackers *Driver::_trackers;

void Driver::init(Setting *setting) {
  _setting = setting;
  _ledProtocol = new LedProtocol(_setting->board.pin.led);
  if (LOG) {
    Serial.begin(_setting->board.serial.baudRate);
    Log.begin(LOG_LEVEL, &Serial);
    delay(2000);
  }
  if (!assertSetting(_setting)) {
    Log.fatal("Invalid setting");
    _ledProtocol->fatalError();
  }
  Log.noticeln("Setting is valid");
  #ifdef BOARD_ESP32
  logSetting(_setting);
  #endif
   Log.noticeln("Waiting before starting...");
  _ledProtocol->waiting();
  _trackers = new Trackers(_setting, _ledProtocol);
  _trackers->init();
  Log.noticeln("Starting program");
  Log.noticeln("JO Sun Tracker - version %s", _setting->program.version);
}

void Driver::watchDog() {
  unsigned long currentTime = millis();
  static unsigned long lastTime = 0;
  if (currentTime - lastTime >= WATCHDOG_INTERVAL) {
    lastTime = currentTime;
    Serial.println("Watchdog");
  }
}

void Driver::update() {
  _trackers->update();
  #if defined(WATCHDOG_INTERVAL)
    if (LOG && WATCHDOG) { watchDog(); }
  #endif
}
