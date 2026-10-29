#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include "setting.h"

class Display {
public:
  bool init(const SettingBoardPinDisplay &pins, const SettingProgramDisplay &display);
  void showLdrs(uint8_t ldrUp, uint8_t ldrDown);

private:
  Adafruit_SSD1306 *_oled = nullptr;
  unsigned long _lastRefresh = 0;
};

#endif
