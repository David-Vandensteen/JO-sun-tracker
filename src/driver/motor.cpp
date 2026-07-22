#include <Arduino.h>
#include <ArduinoLog.h>
#include "motor.h"
#include "setting.h"

Motor::Motor(uint8_t in1, uint8_t in2, uint8_t en, uint8_t pwmResolution)
    : _in1(in1), _in2(in2), _en(en), _pwmResolution(pwmResolution) {}

void Motor::init() {
  Log.traceln("Motor::init");
  pinMode(_in1, OUTPUT);
  pinMode(_in2, OUTPUT);
  pinMode(_en, OUTPUT);
  stop();
}

bool Motor::isBusy() { return _isBusy; }

void Motor::deploy(int speedPercent) {
  Log.traceln("Motor::deploy");
  digitalWrite(_in1, HIGH);
  digitalWrite(_in2, LOW);
  analogWrite(_en, map(speedPercent, 0, 100, 0, _pwmResolution));
  _isBusy = true;
}

void Motor::retract(int speedPercent) {
  Log.traceln("Motor::retract");
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, HIGH);
  analogWrite(_en, map(speedPercent, 0, 100, 0, _pwmResolution));
  _isBusy = true;
}

void Motor::stop() {
  Log.traceln("Motor::stop");
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, LOW);
  analogWrite(_en, 0);
  _isBusy = false;
}