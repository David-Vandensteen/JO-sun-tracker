#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "led_protocol.h"
#include "trackers.h"

Trackers::Trackers(Setting *setting, LedProtocol *ledProtocol) : _setting(setting), _ledProtocol(ledProtocol) {}

void Trackers::init() {
  Log.traceln("Trackers::init");
  Log.noticeln("Initializing trackers");
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    _trackers[i] = new Tracker(
      &_setting->board.pin.tracker[i],
      _setting->board.adc.resolution,
      _setting->board.pwm.resolution,
      _setting->program.motor.speed,
      _setting->program.ldr.threshold
    );
  }
  #if defined(BOARD_UNO) || defined(BOARD_NANO) // TODO
    pinMode(LED_BUILTIN, OUTPUT);
  #endif
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    _trackers[i]->init();
  }
  Log.noticeln("Selected tracker: %d", _command->getSelectedTrackerId());
}

void Trackers::update() {
  _ledProtocol->update();
  Tracker *selectedTracker = _trackers[_command->getSelectedTrackerId()];
  bool deploy = _command->isDeployButtonPressed();
  bool retract = _command->isRetractButtonPressed();
  bool autoMode = _command->isAutoButtonPressed();

  if (autoMode) {
    Log.traceln("Trackers::update auto button pressed");
    selectedTracker->scan();
  }

  if (deploy && retract) {
    Log.traceln("Trackers::update both deploy and retract buttons pressed, stopping tracker");
    selectedTracker->setAutoMode(false);
    selectedTracker->stop();
  }

  if (deploy || retract) {
    if (deploy) {
      Log.traceln("Trackers::update deploy button pressed");
      Log.noticeln("Deploying tracker %d", _command->getSelectedTrackerId());
    }
    if (retract) {
      Log.traceln("Trackers::update retract button pressed");
      Log.noticeln("Retracting tracker %d", _command->getSelectedTrackerId());
    }
    selectedTracker->setAutoMode(false);
    deploy ? selectedTracker->deploy() : selectedTracker->retract();
  }
}
