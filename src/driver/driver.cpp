#include <Arduino.h>
#include <ArduinoLog.h>
#include <esp_system.h>
#include "setting.h"
#include "driver.h"
#include "driver_state.h"
#include "driver_command_state.h"
#include "led_protocol.h"
#include "network_service.h"
#include "log_buffer.h"
#include "trackers.h"
#include "status.h"

static LogOutput logOutput;

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
    Log.begin(LOG_LEVEL, &logOutput);
    delay(10000);
  }
  Log.noticeln("---------");
  if (!assertSetting()) {
    Log.fatal("Invalid setting");
    _ledProtocol->fatalError();
  }
  Log.noticeln("Setting is valid");
  logSetting();
   Log.noticeln("Waiting before starting...");
  _ledProtocol->waiting();
  _trackers = new Trackers(_setting, _ledProtocol);
  _trackers->init();
  Log.noticeln("Starting program");
  Log.noticeln("JO Sun Tracker - version %s", _setting->program.version);
  _command = new DriverCommand(&_setting->board.pin.driver.command, &_setting->program.command.stop);
  _command->init();
  _state.setIdle();
  NetworkService::start(&_setting->secret.wifi);
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
  _command->update();
  DriverCommandState commandState = _command->state();

  if (commandState.isStop() && !_state.isStop()) {
    stop();
  }
  if (commandState.isReset()) {
    _state.setReset();
  }
  if (!_state.isStop()) {
    _trackers->update();
  }
  publishStatus();
  #if defined(HEARTBEAT_INTERVAL)
    if (LOG && HEARTBEAT) { heartBeat(); }
  #endif
}

void Driver::publishStatus() {
  SystemStatus status;
  status.driver = _state.isStop() ? "stop" : (_state.isReset() ? "reset" : "idle");
  status.tracker = _trackers->status();
  NetworkService::publish(status);
}

void Driver::stop() {
  Log.traceln("Driver::stop");
  _trackers->stop();
  _state.setStop();
}

void Driver::reset() {
  Log.traceln("Driver::reset");
  _state.setReset();
  ESP.restart();
}

DriverState Driver::state() {
  return _state;
}
