#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "led_protocol.h"
#include "trackers.h"

Trackers::Trackers(Setting *setting) : _setting(setting) {}

void Trackers::init() {
  Log.trace("Trackers::init\n");
  Log.notice("Initializing trackers\n");
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    _trackers[i] = new Tracker(
      &_setting->board.pin.tracker[i],
      _setting->board.adc.resolution,
      _setting->board.pwm.resolution,
      _setting->program.motor.speed,
      _setting->program.ldr.threshold
    );
  }
  _ledProtocol = new LedProtocol(_setting->board.pin.ledStatus);
  _command = new Command(&_setting->board.pin.button);

  #if defined(BOARD_UNO) || defined(BOARD_NANO)
    pinMode(LED_BUILTIN, OUTPUT);
  #endif
  if (!isValidSetting(_setting)) {
    Log.fatal("Invalid setting");
    _ledProtocol->fatalError();
  }
  Log.notice("Setting is valid\n");
  #ifdef BOARD_ESP32_WROOM_32S
  logSetting(_setting);
  #endif
  Log.notice("Waiting before starting...\n");
  _ledProtocol->waiting();
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    _trackers[i]->init();
  }
  _command->init();
  Log.notice ("Selected tracker: %d\n", _command->getSelectedTrackerId());
}

void Trackers::update() {
  _ledProtocol->update();
  Tracker *selectedTracker = _trackers[_command->getSelectedTrackerId()];
  bool deploy = _command->isDeployButtonPressed();
  bool retract = _command->isRetractButtonPressed();
  bool autoMode = _command->isAutoButtonPressed();

  if (autoMode) {
    Log.trace("Trackers::update auto button pressed\n");
    selectedTracker->scan();
  }

  if (deploy && retract) {
    Log.trace("Trackers::update both deploy and retract buttons pressed, stopping\n");
    selectedTracker->setAutoMode(false);
    selectedTracker->stop();
  }

  if (deploy || retract) {
    if (deploy) {
      Log.trace("Trackers::update deploy button pressed\n");
      Log.notice("Deploying tracker %d\n", _command->getSelectedTrackerId());
    }
    if (retract) {
      Log.trace("Trackers::update retract button pressed\n");
      Log.notice("Retracting tracker %d\n", _command->getSelectedTrackerId());
    }
    selectedTracker->setAutoMode(false);
    deploy ? selectedTracker->deploy() : selectedTracker->retract();
  }
}
