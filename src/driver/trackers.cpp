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

bool Trackers::isAutoMode() {
  return _isAutoMode;
}

void Trackers::setAutoMode(bool autoMode) {
  _isAutoMode = autoMode;
  if (DEBUG) {
    if (_isAutoMode) {
      Serial.println("Trackers: mode AUTO");
    } else {
      Serial.println("Trackers: mode MANUEL");
    }
  }
}

void Trackers::update() {
  bool deployButton = !_setting->board.pin.button.deploy;
  bool retractButton = !_setting->board.pin.button.retract;
  bool scanButton = !_setting->board.pin.button.scan;

  if (deployButton || retractButton) {
    if (DEBUG && deployButton) Serial.println("Trackers::update deploy button pressed");
    if (DEBUG && retractButton) Serial.println("Trackers::update retract button pressed");
    setAutoMode(false);
  }

  if (scanButton) {
    if (DEBUG) Serial.println("Trackers::update scan button pressed");
    setAutoMode(true);
  }

  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    _isAutoMode
      ? _trackers[i].updateAutoMode()
      : _trackers[i].updateManualMode(deployButton, retractButton);
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
