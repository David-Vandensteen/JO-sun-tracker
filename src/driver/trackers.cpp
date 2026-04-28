#include <Arduino.h>
#include "setting.h"
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
}

void Trackers::init() {
  LOG_DEBUG("Trackers::init");
  #if defined(BOARD_UNO)
    pinMode(LED_BUILTIN, OUTPUT);
  #endif
  if (!isValidSetting(_setting)) {
    LOG_ERROR("Invalid setting");
    // TODO implement LED protocol to indicate invalid settings
    while (true);
  }
  waitReady();
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    _trackers[i].init();
  }
}

void Trackers::update() {
  auto selected = _trackers[_setting->board.pin.button.selectedTracker];
  bool deploy = !_setting->board.pin.button.deploy;
  bool retract = !_setting->board.pin.button.retract;
  bool scan = !_setting->board.pin.button.scan;

  if (scan) {
    LOG_DEBUG("Trackers::update scan button pressed");
    selected.scan();
  }

  if (deploy && retract) {
    LOG_DEBUG("Trackers::update both deploy and retract buttons pressed, stopping");
    selected.setAutoMode(false);
    selected.stop();
  }

  if (deploy || retract) {
    if (deploy) LOG_DEBUG("Trackers::update deploy button pressed");
    if (retract) LOG_DEBUG("Trackers::update retract button pressed");
    selected.setAutoMode(false);
    deploy ? selected.deploy() : selected.retract();
  }
}

void Trackers::waitReady() {
  LOG_DEBUG("Trackers::waitReady");
  for (uint8_t i = 0; i < 3; i++) {
    digitalWrite(_setting->board.pin.ledStatus, HIGH);
    #if defined(BOARD_UNO)
      digitalWrite(LED_BUILTIN, HIGH);
    #endif
    delay(1000);
    digitalWrite(_setting->board.pin.ledStatus, LOW);
    #if defined(BOARD_UNO)
      digitalWrite(LED_BUILTIN, LOW);
    #endif
    delay(1000);
  }
}
