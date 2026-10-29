#ifndef APP_STATUS_H
#define APP_STATUS_H
#include <Arduino.h>

// Instantané publié par la boucle principale (cœur 1) et lu par le serveur web (cœur 0).
struct AppTrackerStatus {
  uint8_t ldrUp;
  uint8_t ldrDown;
  const char *mode;
  const char *motor;
  unsigned long nextComparisonMs;
};

// Raw level of each physical control, as read on the pin
struct AppButtonsStatus {
  const char *deploy;
  const char *retract;
  const char *mode;
};

struct AppSystemStatus {
  const char *driver;
  AppTrackerStatus tracker;
  AppButtonsStatus buttons;
};

#endif
