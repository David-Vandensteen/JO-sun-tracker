#ifndef DRIVER_H
#define DRIVER_H

class Driver {
public:
  static void serial(unsigned long baudRate, int logLevel);
  static void log(char *version);
  static void watchDog();
};

#endif