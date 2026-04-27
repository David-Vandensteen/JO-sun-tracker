#include "settings.h"
#include "trackers.h"

Trackers::Trackers(Settings *settings)
  : _settings(settings) {
    for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    _trackers[i] = Tracker(&_settings->board.pin.tracker[i], _settings->board.adc.resolution, _settings->board.pwm.resolution);
  }
}

void Trackers::init() {
  if (DEBUG) Serial.println("Trackers::init");
  if (!isValidSettings(_settings)) {
    if (DEBUG) Serial.println("Invalid settings");
    // TODO led error indication
    while (TRUE);
  }
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
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    _isAutoMode ? _trackers[i].updateAutoMode() : _trackers[i].updateManualMode();
  }
}