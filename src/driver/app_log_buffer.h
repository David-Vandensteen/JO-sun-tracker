#ifndef APP_LOG_BUFFER_H
#define APP_LOG_BUFFER_H
#include <Arduino.h>

#define LOG_BUFFER_SIZE 4096

// Sends log output to Serial and keeps the most recent bytes in a ring buffer
class AppLogOutput : public Print {
public:
  size_t write(uint8_t c) override;
  size_t write(const uint8_t *buffer, size_t size) override;
  static size_t copyTo(char *out, size_t outSize);
};

#endif
