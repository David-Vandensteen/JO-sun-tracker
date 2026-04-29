#include <Arduino.h>

#include "motor.h"
#include "setting.h"
#include <ArduinoLog.h>

Motor::Motor(uint8_t in1, uint8_t in2, uint8_t en, int pwmResolution)
    : _in1(in1), _in2(in2), _en(en), _pwmResolution(pwmResolution) {}

void Motor::init() {
    Log.trace("Motor::init\n");
  stop();
}
bool Motor::isBusy() {
    return _isBusy;
}

void Motor::deploy(int speedPercent) {
    Log.trace("Motor::deploy\n");
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
    analogWrite(_en, map(speedPercent, 0, 100, 0, _pwmResolution));
    _isBusy = true;
}

void Motor::retract(int speedPercent) {
    Log.trace("Motor::retract\n");
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
    analogWrite(_en, map(speedPercent, 0, 100, 0, _pwmResolution));
    _isBusy = true;
}

void Motor::stop() {
    Log.trace("Motor::stop\n");
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, LOW);
    analogWrite(_en, 0);
    _isBusy = false;
}

Motors::Motors(Motor m1, Motor m2)
    : motor1(m1), motor2(m2) {}

void Motors::init() {
    motor1.init();
    motor2.init();
}

void Motors::deploy(int speedPercent) {
    motor1.deploy(speedPercent);
    motor2.deploy(speedPercent);
}

void Motors::retract(int speedPercent) {
    motor1.retract(speedPercent);
    motor2.retract(speedPercent);
}

void Motors::stop() {
    motor1.stop();
    motor2.stop();
}

