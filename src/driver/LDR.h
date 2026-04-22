#ifndef LDR_H
#define LDR_H
#include <Arduino.h>

class LDR {
public:
  int raw;
  int percent;
  LDR(uint8_t pin, int analogResolution);
  void read();
private:
  uint8_t _pin;
  int _analogResolution;
};

class LDRs {
public:
  LDR dayUp;
  LDR dayDown;
  LDR night;
  LDRs(LDR dayUp, LDR dayDown, LDR night);
  void read();
};

#endif