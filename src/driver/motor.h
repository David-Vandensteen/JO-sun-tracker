#ifndef MOTOR_H
#define MOTOR_H
#include <Arduino.h>

class Motor {
public:
	explicit Motor(
    uint8_t in1,
    uint8_t in2,
    uint8_t en,
    uint8_t pwmResolution
  );
  void init();
	void deploy(uint8_t speedPercent);
	void retract(uint8_t speedPercent);
	void stop();
	bool isBusy();

private:
	uint8_t _in1, _in2, _en;
  uint8_t _pwmResolution;
  bool _isBusy = false;
};


#endif