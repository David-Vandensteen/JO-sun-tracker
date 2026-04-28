#include <Arduino.h>
#include "setting.h"
#include "trackers.h"

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
  if (DEBUG) Serial.begin(_setting->board.serial.baudRate);
  if (DEBUG) delay(2000);
  if (DEBUG) Serial.println("Trackers::init");
  if (!isValidSetting(_setting)) {
    if (DEBUG) Serial.println("Invalid setting");
    // TODO implement LED protocol to indicate invalid settings
    while (TRUE);
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
    if (DEBUG) Serial.println("Trackers::update scan button pressed");
    selected.scan();
  }

  if (deploy && retract) {
    if (DEBUG) Serial.println("Trackers::update both deploy and retract buttons pressed, stopping");
    selected.setAutoMode(false);
    selected.stop();
  }

  if (deploy || retract) {
    if (DEBUG) {
      if (deploy) Serial.println("Trackers::update deploy button pressed");
      if (retract) Serial.println("Trackers::update retract button pressed");
    }
    selected.setAutoMode(false);
    deploy ? selected.deploy() : selected.retract();
  }
}

void Trackers::waitReady() {
  if (DEBUG) Serial.println("Trackers::waitReady");
  for (uint8_t i = 0; i < 3; i++) {
    digitalWrite(_setting->board.pin.ledStatus, HIGH);
    delay(1000);
    digitalWrite(_setting->board.pin.ledStatus, LOW);
    delay(1000);
  }
}
