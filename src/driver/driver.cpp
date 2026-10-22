#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "driver.h"
#include "led_protocol.h"
#include "commands.h"
#include "trackers.h"
#include "state.h"

Setting *Driver::_setting;
LedProtocol *Driver::_ledProtocol;
Trackers *Driver::_trackers;

void Driver::init(Setting *setting) {
  _setting = setting;
  _ledProtocol = new LedProtocol(_setting->board.pin.led);
  if (LOG) {
    Serial.begin(_setting->board.serial.baudRate);
    Log.begin(LOG_LEVEL, &Serial);
    delay(10000);
  }
  Log.noticeln("---------");
  if (!assertSetting()) {
    Log.fatal("Invalid setting");
    _ledProtocol->fatalError();
  }
  Log.noticeln("Setting is valid");
  #ifdef BOARD_ESP32
  logSetting();
  #endif
   Log.noticeln("Waiting before starting...");
  _ledProtocol->waiting();
  _trackers = new Trackers(_setting, _ledProtocol);
  _trackers->init();
  Log.noticeln("Starting program");
  Log.noticeln("JO Sun Tracker - version %s", _setting->program.version);
}

void Driver::heartBeat() {
  unsigned long currentTime = millis();
  static unsigned long lastTime = 0;
  if (currentTime - lastTime >= HEARTBEAT_INTERVAL) {
    lastTime = currentTime;
    Serial.println("Heartbeat");
  }
}

void Driver::update() {
  _trackers->update();
  #if defined(HEARTBEAT_INTERVAL)
    if (LOG && HEARTBEAT) { heartBeat(); }
  #endif
}
