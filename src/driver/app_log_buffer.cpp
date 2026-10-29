#include "app_log_buffer.h"

// Written from both cores (loop and network task), so every access is guarded
static portMUX_TYPE logMux = portMUX_INITIALIZER_UNLOCKED;
static char *logData = nullptr;
static size_t logSize = 0;
static size_t logWritePos = 0;
static size_t logCount = 0;

void AppLogOutput::init(size_t size) {
  logData = (char *)malloc(size);
  logSize = logData != nullptr ? size : 0;
}

static void appendByte(uint8_t c) {
  if (logSize == 0) {
    return;
  }
  portENTER_CRITICAL(&logMux);
  logData[logWritePos] = (char)c;
  logWritePos = (logWritePos + 1) % logSize;
  if (logCount < logSize) {
    logCount++;
  }
  portEXIT_CRITICAL(&logMux);
}

size_t AppLogOutput::write(uint8_t c) {
  Serial.write(c);
  appendByte(c);
  return 1;
}

size_t AppLogOutput::write(const uint8_t *buffer, size_t size) {
  for (size_t i = 0; i < size; i++) {
    write(buffer[i]);
  }
  return size;
}

// Copies the most recent bytes in order, keeping room for a terminating NUL
size_t AppLogOutput::copyTo(char *out, size_t outSize) {
  if (outSize == 0) {
    return 0;
  }
  if (logSize == 0) {
    out[0] = '\0';
    return 0;
  }
  portENTER_CRITICAL(&logMux);
  size_t n = logCount < outSize - 1 ? logCount : outSize - 1;
  size_t start = (logWritePos + logSize - n) % logSize;
  for (size_t i = 0; i < n; i++) {
    out[i] = logData[(start + i) % logSize];
  }
  portEXIT_CRITICAL(&logMux);
  out[n] = '\0';
  return n;
}
