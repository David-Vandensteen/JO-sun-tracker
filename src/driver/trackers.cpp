#include <Arduino.h>
#include "setting.h"
#include "led_protocol.h"
#include "trackers.h"
#include "log.h"

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
  LOG_DEBUG("Trackers::init");
  _ledProtocol.init();
  #if defined(BOARD_UNO) || defined(BOARD_NANO)
    pinMode(LED_BUILTIN, OUTPUT);
  #endif
  if (!isValidSetting(_setting)) {
    LOG_ERROR("Invalid setting");
    _ledProtocol.invalidSetting();
  }
  #ifndef BOARD_NANO
  logSetting(_setting);
  #endif
  _ledProtocol.waitReady();
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    _trackers[i].init();
  }
  _command.init();
}

void Trackers::update() {
  Tracker selectedTracker = _trackers[_command.getSelectedTrackerId()];
  bool deploy = _command.isDeployPressed();
  bool retract = _command.isRetractPressed();
  bool scan = _command.isScanPressed();

  if (scan) {
    LOG_DEBUG("Trackers::update scan button pressed");
    selectedTracker.scan();
  }

  if (deploy && retract) {
    LOG_DEBUG("Trackers::update both deploy and retract buttons pressed, stopping");
    selectedTracker.setAutoMode(false);
    selectedTracker.stop();
  }

  if (deploy || retract) {
    if (deploy) LOG_DEBUG("Trackers::update deploy button pressed");
    if (retract) LOG_DEBUG("Trackers::update retract button pressed");
    selectedTracker.setAutoMode(false);
    deploy ? selectedTracker.deploy() : selectedTracker.retract();
  }
}
