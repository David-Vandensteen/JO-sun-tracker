#ifndef SETUP_H
#define SETUP_H
#include "settings.h"

class Setup {
public:
  static void pin(SettingsPin pin);
  static void blinkStatusLED(uint8_t pin);
};

#endif