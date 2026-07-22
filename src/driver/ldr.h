#ifndef LDR_H
#define LDR_H
#include <Arduino.h>

class Ldr {
public:
  int raw;
  uint8_t percent;
  explicit Ldr(uint8_t pin, uint16_t adcResolution);
  void init();
  void update();
private:
  uint16_t _adcResolution;
  uint8_t _pin;
};

class Ldrs {
public:
  Ldr dayUp;
  Ldr dayDown;
  explicit Ldrs(Ldr dayUp, Ldr dayDown);
  void init();
  void update();
};

#endif