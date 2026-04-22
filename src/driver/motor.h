#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

class Motor {
public:
	Motor(uint8_t in1, uint8_t in2, uint8_t en, int pwmResolution);
	void deploy(int speedPercent);
	void retract(int speedPercent);
  void stop();
private:
	uint8_t _in1, _in2, _en;
  int _pwmResolution;
};

class Motors {
public:
	Motors(Motor m1, Motor m2);
	void deploy(int speedPercent);
	void retract(int speedPercent);
  void stop();
	Motor motor1;
	Motor motor2;
};

#endif