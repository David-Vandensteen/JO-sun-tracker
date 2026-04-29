#ifndef LDR_H
#define LDR_H
#include <Arduino.h>

class LDR {
public:
  int raw;
  int percent;
  explicit LDR(uint8_t pin, int analogResolution);
  void init();
  void update();
private:
  uint8_t _pin;
  int _analogResolution;
};

class LDRs {
public:
  LDR dayUp;
  LDR dayDown;
  LDR back;
  explicit LDRs(LDR dayUp, LDR dayDown, LDR back);
  void init();
  void update();
  bool isDayUpDifferentFromDayDown(long threshold);
  bool isDayUpBrighterThanDayDown(long threshold);
  bool isDayDownBrighterThanDayUp(long threshold);
};

#endif