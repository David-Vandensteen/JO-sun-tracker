#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "led_protocol.h"
#include "trackers.h"

Trackers::Trackers(Setting *setting, LedProtocol *ledProtocol) : _setting(setting), _ledProtocol(ledProtocol) {}

void Trackers::init() {
  Log.traceln("Trackers::init");
  Log.noticeln("Initializing trackers");
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    _trackers[i] = new Tracker(
      &_setting->board.pin.tracker[i],
      &_setting->board.pin.tracker[i].command,
      _setting->board.adc.resolution,
      _setting->board.pwm.resolution,
      _setting->program.ldr.threshold,
      _setting->program.motor.speed
    );
  }
  #if defined(BOARD_UNO) || defined(BOARD_NANO) // TODO
    pinMode(LED_BUILTIN, OUTPUT);
  #endif
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    _trackers[i]->init();
  }
}

void Trackers::update() {
  _ledProtocol->update();
  // TODO command stop/reset
}
