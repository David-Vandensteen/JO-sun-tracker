#ifndef LOG_H
#define LOG_H
#include <Arduino.h>

#define LOG_SERIAL true

#define LOG_LEVEL_ERROR  0x01
#define LOG_LEVEL_WARN   0x02
#define LOG_LEVEL_INFO   0x04
#define LOG_LEVEL_DEBUG  0x08

#define LOG_LEVEL_MASK (LOG_LEVEL_ERROR | LOG_LEVEL_WARN | LOG_LEVEL_INFO | LOG_LEVEL_DEBUG)

#define BUFFER_SIZE 64

#if LOG_SERIAL
#define LOG_ERROR(msg)   do { if (LOG_LEVEL_MASK & LOG_LEVEL_ERROR) { Serial.print("[ERROR] "); Serial.println(msg); } } while(0)
#define LOG_WARN(msg)    do { if (LOG_LEVEL_MASK & LOG_LEVEL_WARN)  { Serial.print("[WARN]  "); Serial.println(msg); } } while(0)
#define LOG_INFO(msg)    do { if (LOG_LEVEL_MASK & LOG_LEVEL_INFO)  { Serial.print("[INFO]  "); Serial.println(msg); } } while(0)
#define LOG_DEBUG(msg)   do { if (LOG_LEVEL_MASK & LOG_LEVEL_DEBUG) { Serial.print("[DEBUG] "); Serial.println(msg); } } while(0)

#define LOG_ERRORF(fmt, ...)   do { if (LOG_LEVEL_MASK & LOG_LEVEL_ERROR) { char buf[BUFFER_SIZE]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print("[ERROR] "); Serial.println(buf); } } while(0)
#define LOG_WARNF(fmt, ...)    do { if (LOG_LEVEL_MASK & LOG_LEVEL_WARN)  { char buf[BUFFER_SIZE]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print("[WARN]  "); Serial.println(buf); } } while(0)
#define LOG_INFOF(fmt, ...)    do { if (LOG_LEVEL_MASK & LOG_LEVEL_INFO)  { char buf[BUFFER_SIZE]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print("[INFO]  "); Serial.println(buf); } } while(0)
#define LOG_DEBUGF(fmt, ...)   do { if (LOG_LEVEL_MASK & LOG_LEVEL_DEBUG) { char buf[BUFFER_SIZE]; snprintf(buf, sizeof(buf), fmt, ##__VA_ARGS__); Serial.print("[DEBUG] "); Serial.println(buf); } } while(0)
#else
#define LOG_ERROR(msg)   ((void)0)
#define LOG_WARN(msg)    ((void)0)
#define LOG_INFO(msg)    ((void)0)
#define LOG_DEBUG(msg)   ((void)0)
#define LOG_ERRORF(fmt, ...)   ((void)0)
#define LOG_WARNF(fmt, ...)    ((void)0)
#define LOG_INFOF(fmt, ...)    ((void)0)
#define LOG_DEBUGF(fmt, ...)   ((void)0)
#endif

#endif
