#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>
#include "motor_state.h"

class Motor {
public:
	explicit Motor(
    uint8_t in1,
    uint8_t in2,
    uint8_t en,
    uint8_t pwmResolution
  );
  void init();
	void deploy(uint8_t speed);
  void deployWithTimeOut(uint8_t speed, unsigned long timeout);
	void retract(uint8_t speed);
  void retractWithTimeOut(uint8_t speed, unsigned long timeout);
	void stop();
  void update();
  MotorState state();

private:
  unsigned long _lastActionTime = 0;
  unsigned long _timeout = 0;
	uint8_t _in1, _in2, _en;
  uint8_t _pwmResolution;
  MotorState _state;
};

#endif