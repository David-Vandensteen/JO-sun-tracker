#include <Arduino.h>
#include <ArduinoLog.h>
#include <Wire.h>
#include "display.h"

static const unsigned long DISPLAY_REFRESH_INTERVAL = 500;

bool Display::init(const SettingBoardPinDisplay &pins, const SettingProgramDisplay &display) {
  Wire.begin(pins.sda, pins.scl);
  _oled = new Adafruit_SSD1306(display.width, display.height, &Wire);
  if (!_oled->begin(SSD1306_SWITCHCAPVCC, display.address)) {
    Log.warningln("Display: SSD1306 not found at 0x%X, display disabled", display.address);
    delete _oled;
    _oled = nullptr;
    return false;
  }
  _oled->clearDisplay();
  _oled->display();
  return true;
}

void Display::showLdrs(uint8_t ldrUp, uint8_t ldrDown) {
  if (_oled == nullptr) {
    return;
  }
  unsigned long now = millis();
  if (now - _lastRefresh < DISPLAY_REFRESH_INTERVAL) {
    return;
  }
  _lastRefresh = now;

  _oled->clearDisplay();
  _oled->setTextColor(SSD1306_WHITE);
  _oled->setTextSize(1);
  _oled->setCursor(0, 0);
  _oled->println(F("JO Sun Tracker"));
  _oled->setTextSize(2);
  _oled->setCursor(0, 20);
  _oled->printf("UP   %3u %%\n", ldrUp);
  _oled->printf("DOWN %3u %%", ldrDown);
  _oled->display();
}
