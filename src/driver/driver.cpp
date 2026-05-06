#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "driver.h"
#include "led_protocol.h"
#include "command.h"
#include "trackers.h"

Setting *Driver::_setting;
LedProtocol *Driver::_ledProtocol;
Command *Driver::_command;
Trackers *Driver::_trackers;

void Driver::init(Setting *setting) {
  _setting = setting;
  _command = new Command(&_setting->board.pin.button);
  _ledProtocol = new LedProtocol(_setting->board.pin.led);
  if (!assertSetting(_setting)) {
    Log.fatal("Invalid setting");
    _ledProtocol->fatalError();
  }
  Log.notice("Setting is valid\n");
  #ifdef BOARD_ESP32_WROOM_32S
  logSetting(_setting);
  #endif
   Log.notice("Waiting before starting...\n");
  _ledProtocol->waiting();
  _command->init();
  _trackers = new Trackers(_setting, _ledProtocol);
  _trackers->init();
  if (LOG) {
    Serial.begin(_setting->board.serial.baudRate);
    Log.begin(LOG_LEVEL, &Serial);
    delay(2000);
  }
  Log.notice("Starting program\n");
  Log.notice("JO Sun Tracker - version %s\n", _setting->program.version);
}

void Driver::watchDog() {
  if (LOG && WATCHDOG) {
    unsigned long currentTime = millis();
    static unsigned long lastTime = 0;
    if (currentTime - lastTime >= WATCHDOG_INTERVAL) {
      lastTime = currentTime;
      Serial.println("Watchdog");
    }
  }
}

void Driver::update() {
  _trackers->update();
  #if defined(WATCHDOG_INTERVAL)
    watchDog();
  #endif
}
