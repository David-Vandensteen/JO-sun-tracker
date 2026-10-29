#ifndef APP_LOG_BUFFER_H
#define APP_LOG_BUFFER_H
#include <Arduino.h>

// Ring buffer of log output, size set from program.app.log.bufferSize
class AppLogOutput : public Print {
public:
  static void init(size_t size);
  size_t write(uint8_t c) override;
  size_t write(const uint8_t *buffer, size_t size) override;
  static size_t copyTo(char *out, size_t outSize);
};

#endif
