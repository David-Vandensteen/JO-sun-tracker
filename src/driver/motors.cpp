#include "motor.h"
#include "motors.h"

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
