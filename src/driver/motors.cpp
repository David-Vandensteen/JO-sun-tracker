#include <Arduino.h>
#include "setting.h"
#include "motor.h"
#include "motors.h"

// Public
Motors::Motors(
  Motor motor1,
  Motor motor2,
  SettingProgramMotors *settingProgramMotors
)
  : _motor1(motor1),
    _motor2(motor2),
    _speed(settingProgramMotors->speed),
    _timeout(settingProgramMotors->timeout) {}

void Motors::init() {
  _motor1.init();
  _motor2.init();
  stop();
}

void Motors::deploy() {
  if (_timeout > 0) {
    _motor1.deployWithTimeOut(_speed, _timeout);
    _motor2.deployWithTimeOut(_speed, _timeout);
  } else {
    _motor1.deploy(_speed);
    _motor2.deploy(_speed);
  }
}

void Motors::retract() {
  stop();
  if (_timeout > 0) {
    _motor1.retractWithTimeOut(_speed, _timeout);
    _motor2.retractWithTimeOut(_speed, _timeout);
  } else {
    _motor1.retract(_speed);
    _motor2.retract(_speed);
  }
}

void Motors::setPositionZero() {
  stop();
  // TODO: Implement the logic to set the motors' position to zero if applicable.
}

void Motors::stop() {
  _motor1.stop();
  _motor2.stop();
}

void Motors::update() {
  _motor1.update();
  _motor2.update();
}

MotorState Motors::state() {
  return _motor1.state();
}
