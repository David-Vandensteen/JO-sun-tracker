#include <Arduino.h>
#include <ArduinoLog.h>
#include <dv_every_interval.h>
#include "command.h"
#include "setting.h"
#include "tracker.h"
#include "ldr.h"
#include "ldrs.h"
#include "motor.h"
#include "state.h"
#include "command_state.h"

// Public
Tracker::Tracker(
  SettingBoardPinTracker *settingBoardPinTracker,
  SettingBoardPinMode *settingBoardPinMode,
  SettingProgramTrackers *settingProgramTrackers,
  SettingProgramLDR *settingProgramLDR,
  SettingProgramLDRs *settingProgramLDRs,
  SettingBoardADC *settingBoardADC,
  SettingBoardPWM *settingBoardPWM,
  SettingProgramMotors *settingProgramMotors
)
  : _settingProgramMotors(settingProgramMotors),
    _settingBoardPinMode(settingBoardPinMode),
    _ldrs(
      Ldr(
        settingBoardPinTracker->ldr.up,
        settingBoardADC->resolution,
        settingProgramLDR
      ),
      Ldr(
        settingBoardPinTracker->ldr.down,
        settingBoardADC->resolution,
        settingProgramLDR
      ),
      settingProgramLDRs
    ),
    _motors(
      Motor(
        settingBoardPinTracker->motors.in1,
        settingBoardPinTracker->motors.in2,
        settingBoardPinTracker->motors.ena,
        settingBoardPWM->resolution
      ),
      Motor(
        settingBoardPinTracker->motors.in3,
        settingBoardPinTracker->motors.in4,
        settingBoardPinTracker->motors.enb,
        settingBoardPWM->resolution
      ),
      settingProgramMotors
    ),
    _command(&settingBoardPinTracker->command),
    _settingProgramTrackers(settingProgramTrackers) {}

void Tracker::init() {
  Log.traceln("Tracker::init - mode: %s", isManualMode() ? "manual" : "auto");
  pinMode(_settingBoardPinMode->manual, INPUT);
  _ldrs.init();
  _motors.init();
  _command.init();
  _everyInterval
    .setInterval(_settingProgramTrackers->interval)
    .setCallback(Tracker::onIntervalTick, this);
  _state = _state.setStateInit();
}

State Tracker::update() {
  CommandState commandState = _command.update();
  if (commandState.isStateStop()) {
    Log.traceln("Tracker::update - Command STOP");
    stop();
    return _state.setStateStopped();
  } else if (commandState.isStateReset()) {
    Log.traceln("Tracker::update - Command RESET");
    stop();
    // TODO: reset behavior
  }
  _everyInterval.update();
  _ldrs.update();
  _motors.update();

  return _state;
}

// Private
void Tracker::deploy() {
  Log.traceln("Tracker::deploy");
  _state.setStateRun();
  _motors.deploy();
}

void Tracker::retract() {
  Log.traceln("Tracker::retract");
  _state.setStateRun();
  _motors.retract();
}

void Tracker::reset() {
  Log.traceln("Tracker::reset");
  _state.setStateReset();
  #ifdef BOARD_ESP32
  #include <esp_system.h>
  ESP.restart();
  #elif defined(BOARD_UNO) || defined(BOARD_NANO)
  #include <avr/wdt.h>
  wdt_enable(WDTO_15MS);
  while (1) {}
  #endif
}

void Tracker::stop() {
  Log.traceln("Tracker::stop");
  _state.setStateStopped();
  _motors.stop();
}

bool Tracker::isManualMode() {
  return digitalRead(_settingBoardPinMode->manual) == HIGH;
}

void Tracker::onIntervalTick(void *ctx) {
  if (ctx == nullptr) {
    return;
  }

  Tracker *self = static_cast<Tracker *>(ctx);
  self->interval();
}

void Tracker::interval() {
  #if LOG_TRACKER_INTERVAL
  Log.traceln("Tracker::interval - mode: %s", isManualMode() ? "manual" : "automa");
  #endif
  if (isManualMode()) {
    if (_state.isStateRun()) {
      stop();
    }
    return;
  }

  LdrsComparison comparison = _ldrs.update();
  switch (comparison) {
  case LdrsComparison::UP_GREATER_THAN_DOWN:
    #if LOG_TRACKER_INTERVAL
    Log.traceln("Tracker::interval - LDR UP_GREATER_THAN_DOWN");
    #endif
    deploy();
    break;
  case LdrsComparison::DOWN_GREATER_THAN_UP:
    #if LOG_TRACKER_INTERVAL
    Log.traceln("Tracker::interval - LDR DOWN_GREATER_THAN_UP");
    #endif
    retract();
    break;
  case LdrsComparison::DEADBAND:
    #if LOG_TRACKER_INTERVAL
    Log.traceln("Tracker::interval - LDR DEADBAND");
    #endif
    if (_state.isStateRun()) {
      stop();
    }
    break;
  case LdrsComparison::NIGHT:
    #if LOG_TRACKER_INTERVAL
    Log.traceln("Tracker::interval - LDR NIGHT");
    #endif
    stop();
    // TODO: night behavior
    break;
  default:
    break;
  }
}
