#ifndef LOG_H
#define LOG_H
#include <Arduino.h>

#define LOG_ERROR(msg)   do { if (DEBUG) { Serial.print("[ERROR] "); Serial.println(msg); } } while(0)
#define LOG_WARN(msg)    do { if (DEBUG) { Serial.print("[WARN]  "); Serial.println(msg); } } while(0)
#define LOG_INFO(msg)    do { if (DEBUG) { Serial.print("[INFO]  "); Serial.println(msg); } } while(0)
#define LOG_DEBUG(msg)   do { if (DEBUG) { Serial.print("[DEBUG] "); Serial.println(msg); } } while(0)

#endif
