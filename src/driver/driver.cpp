#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "driver.h"
#include "led_protocol.h"
#include "trackers.h"
#include "driver_state.h"
#include "driver_command_state.h"

Setting *Driver::_setting;
LedProtocol *Driver::_ledProtocol;
Trackers *Driver::_trackers;
DriverState Driver::_state;
DriverCommand *Driver::_command;

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
  _command = new DriverCommand(&_setting->board.pin.driver.command);
  _command->init();
  _state.setIdle();
}

void Driver::heartBeat() {
  unsigned long currentTime = millis();
  static unsigned long lastTime = 0;
  if (currentTime - lastTime >= HEARTBEAT_INTERVAL) {
    lastTime = currentTime;
    Serial.println("Heartbeat");
  }
}

DriverState Driver::update() {
  DriverCommandState commandState = _command->update();
  if (commandState.isStop()) {
    _state.setStop();
    return _state;
  }
  if (commandState.isReset()) {
    _state.setReset();
    return _state;
  }
  _trackers->update();
  #if defined(HEARTBEAT_INTERVAL)
    if (LOG && HEARTBEAT) { heartBeat(); }
  #endif
  return _state;
}

void Driver::stop() {
  Log.traceln("Driver::stop");
  _trackers->stop();
  _state.setStop();
}

void Driver::reset() {
  Log.traceln("Driver::reset");
  _state.setReset();
  #ifdef BOARD_ESP32
  #include <esp_system.h>
  ESP.restart();
  #elif defined(BOARD_UNO) || defined(BOARD_NANO)
  #include <avr/wdt.h>
  wdt_enable(WDTO_15MS);
  while (1) {}
  #endif
}
