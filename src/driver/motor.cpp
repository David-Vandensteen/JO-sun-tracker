#include <Arduino.h>
#include "motor.h"

Motor::Motor(uint8_t in1, uint8_t in2, uint8_t en)
    : _in1(in1), _in2(in2), _en(en) {}

void Motor::deploy(int speedPercent, int pwmResolution) {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
    analogWrite(_en, map(speedPercent, 0, 100, 0, pwmResolution));
}

void Motor::retract(int speedPercent, int pwmResolution) {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
    analogWrite(_en, map(speedPercent, 0, 100, 0, pwmResolution));
}

Motors::Motors(const Motor& m1, const Motor& m2)
    : motor1(m1), motor2(m2) {}

void Motors::deploy(int speedPercent, int pwmResolution) {
    motor1.deploy(speedPercent, pwmResolution);
    motor2.deploy(speedPercent, pwmResolution);
}

void Motors::retract(int speedPercent, int pwmResolution) {
    motor1.retract(speedPercent, pwmResolution);
    motor2.retract(speedPercent, pwmResolution);
}

