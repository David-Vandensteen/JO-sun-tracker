#ifndef DRIVER_H
#define DRIVER_H
#include <Arduino.h>
#include "led_protocol.h"

class Driver {
public:
  static LedProtocol createLedProtocol(uint8_t pin);
  static void serial(unsigned long baudRate, int logLevel);
  static void log(char *version);
  static void watchDog();
};

#endif