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

void Trackers::update() {
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    _trackers[i].update();
  }
}