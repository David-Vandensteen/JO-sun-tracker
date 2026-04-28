#include <Arduino.h>
#include <ArduinoLog.h>
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
  if (DEBUG) {
    Serial.begin(_setting->board.serial.baudRate);
    delay(2000);
    Log.begin(LOG_LEVEL_NOTICE, &Serial);
    Log.notice("Trackers::init\n");
  }
  if (!isValidSetting(_setting)) {
    if (DEBUG) Log.error("Invalid setting\n");
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
    if (DEBUG) Log.notice("Trackers::update scan button pressed\n");
    selected.scan();
  }

  if (deploy && retract) {
    if (DEBUG) Log.notice("Trackers::update both deploy and retract buttons pressed, stopping\n");
    selected.setAutoMode(false);
    selected.stop();
  }

  if (deploy || retract) {
    if (DEBUG && deploy) Log.notice("Trackers::update deploy button pressed\n");
    if (DEBUG && retract) Log.notice("Trackers::update retract button pressed\n");
    selected.setAutoMode(false);
    deploy ? selected.deploy() : selected.retract();
  }
}

void Trackers::waitReady() {
  if (DEBUG) Log.notice("Trackers::waitReady\n");
  for (uint8_t i = 0; i < 3; i++) {
    digitalWrite(_setting->board.pin.ledStatus, HIGH);
    delay(1000);
    digitalWrite(_setting->board.pin.ledStatus, LOW);
    delay(1000);
  }
}
