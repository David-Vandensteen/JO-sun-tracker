#ifndef MOTORS_H
#define MOTORS_H
#include "motor.h"

class Motors {
public:
	explicit Motors(Motor m1, Motor m2);
  void init();
	void deploy(int speedPercent);
	void retract(int speedPercent);
  void stop();
	Motor motor1;
	Motor motor2;
};

#endif