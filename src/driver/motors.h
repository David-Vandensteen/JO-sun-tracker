#ifndef MOTORS_H
#define MOTORS_H
#include <Arduino.h>
#include "setting.h"
#include "motor.h"
#include "motor_state.h"

class Motors {
public:
	explicit Motors(
    Motor motor1,
    Motor motor2,
    SettingProgramMotors *settingProgramMotors
  );
  void init();
	void deploy();
	void retract();
  void setPositionZero();
  void stop();
  void update();
  MotorState state();

private:
	Motor _motor1;
	Motor _motor2;
  unsigned long _timeout = 0;
  uint8_t _speed;
};

#endif