#ifndef MOTOR_H
#define MOTOR_H

void MotorDeployByPin(uint8_t inPin1, uint8_t inPin2, uint8_t enPin, int speedPercent, int pwmResolution);
void MotorDeployById(uint8_t id, int speedPercent, int pwmResolution);
void MotorsDeploy(int speedPercent, int pwmResolution);
void MotorRetractByPin(uint8_t inPin1, uint8_t inPin2, uint8_t enPin, int speedPercent, int pwmResolution);
void MotorRetractById(uint8_t id, int speedPercent, int pwmResolution);
void MotorsRetract(int speedPercent, int pwmResolution);

#endif