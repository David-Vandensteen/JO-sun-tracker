#ifndef LOG_H
#define LOG_H

#include <Arduino.h>
#include <ArduinoLog.h>

#define LOG true

#define LOG_LEVEL_ERROR      0x01
#define LOG_LEVEL_WARNING    0x02
#define LOG_LEVEL_INFO       0x04
#define LOG_LEVEL_TRACE      0x08

#define LOG_LEVEL_MASK (LOG_LEVEL_ERROR | LOG_LEVEL_WARNING | LOG_LEVEL_INFO | LOG_LEVEL_TRACE)

#if LOG
  #define LOG_ERROR(msg)   if (LOG && (LOG_LEVEL_MASK & LOG_LEVEL_ERROR))    Log.error(msg)
  #define LOG_WARNING(msg) if (LOG && (LOG_LEVEL_MASK & LOG_LEVEL_WARNING))  Log.warning(msg)
  #define LOG_NOTICE(msg)  if (LOG && (LOG_LEVEL_MASK & LOG_LEVEL_INFO))     Log.notice(msg)
  #define LOG_TRACE(msg)   if (LOG && (LOG_LEVEL_MASK & LOG_LEVEL_TRACE))    Log.trace(msg)
#else
  #define LOG_ERROR(msg)   do {} while(0)
  #define LOG_WARNING(msg) do {} while(0)
  #define LOG_NOTICE(msg)  do {} while(0)
  #define LOG_TRACE(msg)   do {} while(0)
#endif
#endif