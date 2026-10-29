#include <Arduino.h>
#include <ArduinoLog.h>
#include <dv_every_interval.h>
#include "setting.h"
#include "tracker.h"
#include "ldr.h"
#include "ldrs.h"
#include "motor.h"

// Public
Tracker::Tracker(
  SettingBoardPinTracker *settingBoardPinTracker,
  SettingBoardPinDriverCommand *settingBoardPinDriverCommand,
  SettingProgramTracker *settingProgramTracker,
  SettingProgramLDR *settingProgramLDR,
  SettingProgramLDRs *settingProgramLDRs,
  SettingBoardADC *settingBoardADC,
  SettingBoardPWM *settingBoardPWM,
  SettingProgramMotors *settingProgramMotors
)
  : _settingProgramMotors(settingProgramMotors),
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
    _trackerCommand(&settingBoardPinTracker->command, &settingProgramTracker->command),
    _settingProgramTracker(settingProgramTracker) {}

void Tracker::init() {
  _trackerCommand.init();
  Log.traceln("Tracker::init - mode: %s", _trackerCommand.state().isAuto() ? "auto" : "manual");
  _ldrs.init();
  _motors.init();
  _everyInterval
    .setInterval(_settingProgramTracker->interval)
    .setCallback(Tracker::onIntervalTick, this);
  _nextComparisonMs = millis() + _settingProgramTracker->interval;
}

void Tracker::update() {
  _everyInterval.update();
  _ldrs.update();
  _motors.update();
  handleCommand();
}

// Private
void Tracker::handleCommand() {
  _trackerCommand.update();
  TrackerCommandState state = _trackerCommand.state();

  // Sortie du mode manuel : arrêt immédiat du mouvement manuel en cours.
  // La commande passe à AUTO, donc un bouton encore maintenu sera pris en compte au retour en manuel.
  if (state.isAuto()) {
    if (_commandState.isDeploy() || _commandState.isRetract()) {
      Log.traceln("Tracker::handleCommand - leaving manual mode, stopping");
      stop();
    }
    _commandState = state;
    return;
  }

  if (state.isDeploy()) {
    _motors.deploy();
  } else if (state.isRetract()) {
    _motors.retract();
  } else if (_commandState.isDeploy() || _commandState.isRetract()) {
    stop();
  }
  _commandState = state;
}

void Tracker::deploy() {
  Log.traceln("Tracker::deploy");
  _motors.deploy();
}

void Tracker::retract() {
  Log.traceln("Tracker::retract");
  _motors.retract();
}


void Tracker::stop() {
  Log.traceln("Tracker::stop");
  _motors.stop();
}

AppTrackerStatus Tracker::AppStatus() {
  AppTrackerStatus status;
  status.ldrUp = _ldrs.ldrUp.getAveragePercentValue();
  status.ldrDown = _ldrs.ldrDown.getAveragePercentValue();
  status.mode = _trackerCommand.state().isAuto() ? "auto" : "manual";
  MotorState motorState = _motors.state();
  status.motor = motorState.isDeploy() ? "deploy" : (motorState.isRetract() ? "retract" : "idle");
  long remainingMs = (long)(_nextComparisonMs - millis());
  status.nextComparisonMs = remainingMs > 0 ? (unsigned long)remainingMs : 0;
  return status;
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
  Log.traceln("Tracker::interval - mode: %s", _trackerCommand.state().isAuto() ? "automa" : "manual");
  #endif
  _nextComparisonMs = millis() + _settingProgramTracker->interval;
  if (!_trackerCommand.state().isAuto()) {
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
