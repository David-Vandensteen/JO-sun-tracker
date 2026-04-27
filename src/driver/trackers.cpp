#include <Arduino.h>
#include "settings.h"
#include "trackers.h"

Trackers::Trackers(Settings *settings)
  : _settings(settings) {
    for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    _trackers[i] = Tracker(
      &_settings->board.pin.tracker[i],
      _settings->board.adc.resolution,
      _settings->board.pwm.resolution,
      _settings->program.motor.speed,
      _settings->program.ldr.threshold
    );
  }
}

void Trackers::init() {
  if (DEBUG) Serial.begin(_settings->board.serial.baudRate);
  if (DEBUG) delay(2000);
  if (DEBUG) Serial.println("Trackers::init");
  if (!isValidSettings(_settings)) {
    if (DEBUG) Serial.println("Invalid settings");
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
      Serial.println("Switched to auto mode");
    } else {
      Serial.println("Switched to manual mode");
    }
  }
}

void Trackers::update() {
  bool deployButton = !digitalRead(_settings->board.pin.button.deploy);
  bool retractButton = !digitalRead(_settings->board.pin.button.retract);
  bool scanButton = !digitalRead(_settings->board.pin.button.scan);

  if (deployButton || retractButton) {
    if (DEBUG && deployButton) Serial.println("Trackers::update deploy button pressed");
    if (DEBUG && retractButton) Serial.println("Trackers::update retract button pressed");
    setAutoMode(false);
  }

  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    _isAutoMode
      ? _trackers[i].updateAutoMode()
      : _trackers[i].updateManualMode(deployButton, retractButton);
  }
}

void Trackers::waitReady() {
  if (DEBUG) Serial.println("Trackers::waitReady");
  for (uint8_t i = 0; i < 3; i++) { // TODO : implement led protocol to indicate waiting for ready state
    digitalWrite(_settings->board.pin.ledStatus, HIGH);
    delay(1000);
    digitalWrite(_settings->board.pin.ledStatus, LOW);
    delay(1000);
  }
}
