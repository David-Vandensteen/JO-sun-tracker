#ifndef LDR_H
#define LDR_H
#include <Arduino.h>

class Ldr {
public:
  int raw;
  int percent;
  explicit Ldr(uint8_t pin, int analogResolution);
  void init();
  void update();
private:
  uint8_t _pin;
  int _analogResolution;
};

class Ldrs {
public:
  Ldr dayUp;
  Ldr dayDown;
  Ldr back;
  explicit Ldrs(Ldr dayUp, Ldr dayDown, Ldr back);
  void init();
  void update();
  bool isDayUpDifferentFromDayDown(long threshold);
  bool isDayUpBrighterThanDayDown(long threshold);
  bool isDayDownBrighterThanDayUp(long threshold);
};

#endif