#include <Arduino.h>
#include <ArduinoLog.h>
#include "motor.h"
#include "setting.h"

// Public
Motor::Motor(uint8_t in1, uint8_t in2, uint8_t en, uint8_t pwmResolution)
    : _in1(in1),
      _in2(in2),
      _en(en),
      _pwmResolution(pwmResolution) {}

void Motor::init() {
  Log.traceln("Motor::init");
  _lastActionTime = 0;
  _timeout = 0;
  _state.setIdle();
  pinMode(_in1, OUTPUT);
  pinMode(_in2, OUTPUT);
  pinMode(_en, OUTPUT);
}

MotorState Motor::state() {
  return _state;
}

void Motor::deploy(uint8_t speed) {
  Log.traceln("Motor::deploy");
  _state.setDeploy();
  digitalWrite(_in1, HIGH);
  digitalWrite(_in2, LOW);
  analogWrite(_en, map(speed, 0, 100, 0, _pwmResolution));
}

void Motor::deployWithTimeOut(uint8_t speed, unsigned long timeout) {
  Log.traceln("Motor::deployWithTimeOut");
  deploy(speed);
  _lastActionTime = millis();
  _timeout = timeout;
}

void Motor::retract(uint8_t speed) {
  Log.traceln("Motor::retract");
  _state.setRetract();
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, HIGH);
  analogWrite(_en, map(speed, 0, 100, 0, _pwmResolution));
}

void Motor::retractWithTimeOut(uint8_t speed, unsigned long timeout) {
  Log.traceln("Motor::retractWithTimeOut");
  retract(speed);
  _lastActionTime = millis();
  _timeout = timeout;
}

void Motor::stop() {
  Log.traceln("Motor::stop");
  _state.setIdle();
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, LOW);
  analogWrite(_en, 0);
  _timeout = 0;
}

void Motor::update() {
  if (_timeout > 0 && (millis() - _lastActionTime >= _timeout)) {
    Log.traceln("Motor::update: timeout reached, stopping motor");
    stop();
  }
}