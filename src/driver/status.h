#ifndef STATUS_H
#define STATUS_H
#include <Arduino.h>

// Instantané publié par la boucle principale (cœur 1) et lu par le serveur web (cœur 0).
struct TrackerStatus {
  uint8_t ldrUp;
  uint8_t ldrDown;
  const char *mode;
  const char *motor;
  unsigned long nextComparisonMs;
};

// Raw level of each physical control, as read on the pin
struct ButtonsStatus {
  const char *deploy;
  const char *retract;
  const char *mode;
  const char *stop;
};

struct SystemStatus {
  const char *driver;
  TrackerStatus tracker;
  ButtonsStatus buttons;
};

#endif
