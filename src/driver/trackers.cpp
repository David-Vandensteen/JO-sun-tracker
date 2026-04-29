#include <Arduino.h>
#include "setting.h"
#include "led_protocol.h"
#include "trackers.h"
#include <ArduinoLog.h>

Trackers::Trackers(Setting *setting)
  : _setting(setting) {
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    _trackers[i] = Tracker(
      &_setting->board.pin.tracker[i],
      _setting->board.adc.resolution,
      _setting->board.pwm.resolution,
      _setting->program.motor.speed,
      _setting->program.ldr.threshold
    );
  }
  _ledProtocol = LedProtocol(_setting->board.pin.ledStatus);
  _command = Command(&_setting->board.pin.button);
}

void Trackers::init() {
  Log.trace("Trackers::init\n");
  Log.notice("Initializing trackers\n");
  _ledProtocol.init();
  #if defined(BOARD_UNO) || defined(BOARD_NANO)
    pinMode(LED_BUILTIN, OUTPUT);
  #endif
  if (!isValidSetting(_setting)) {
    Log.fatal("Invalid setting");
    _ledProtocol.invalidSetting();
  }
  Log.notice("Setting is valid\n");
  #ifdef BOARD_ESP32_WROOM_32S
  logSetting(_setting);
  #endif
  Log.notice("Waiting before starting...\n");
  _ledProtocol.waitReady();
  Log.notice("Now is ready\n");
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    _trackers[i].init();
  }
  _command.init();
}

void Trackers::update() {
  Tracker selectedTracker = _trackers[_command.getSelectedTrackerId()];
  bool deploy = _command.isDeployButtonPressed();
  bool retract = _command.isRetractButtonPressed();
  bool scan = _command.isScanButtonPressed();

  if (scan) {
    Log.trace("Trackers::update scan button pressed\n");
    selectedTracker.scan();
  }

  if (deploy && retract) {
    Log.trace("Trackers::update both deploy and retract buttons pressed, stopping\n");
    selectedTracker.setAutoMode(false);
    selectedTracker.stop();
  }

  if (deploy || retract) {
    if (deploy) Log.trace("Trackers::update deploy button pressed\n");
    if (retract) Log.trace("Trackers::update retract button pressed\n");
    selectedTracker.setAutoMode(false);
    deploy ? selectedTracker.deploy() : selectedTracker.retract();
  }
}
