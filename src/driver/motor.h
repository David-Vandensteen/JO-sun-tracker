#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

class Motor {
public:
	Motor(uint8_t in1, uint8_t in2, uint8_t en);
	void deploy(int speedPercent, int pwmResolution);
	void retract(int speedPercent, int pwmResolution);
private:
	uint8_t _in1, _in2, _en;
};

class Motors {
public:
	Motors(Motor m1, Motor m2);
	void deploy(int speedPercent, int pwmResolution);
	void retract(int speedPercent, int pwmResolution);
	Motor motor1;
	Motor motor2;
};

#endif