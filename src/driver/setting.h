#ifndef SETTING_H
#define SETTING_H
#include <Arduino.h>
#include <ArduinoLog.h>

#define VERSION "0.0.1"

#define TRACKER_MAX 1
#define TRACKER_1 0

// ------------------
//  BOARD
// ------------------
// #define BOARD_NANO
#define BOARD_UNO
// #define BOARD_ESP32

#ifdef BOARD_UNO
#undef TRACKER_MAX
#define TRACKER_MAX 1
#endif

#ifdef BOARD_NANO
#undef TRACKER_MAX
#define TRACKER_MAX 1
#endif

// ------------------
//  LOG
// ------------------
#define LOG true

// #define LOG_LEVEL LOG_LEVEL_SILENT
// #define LOG_LEVEL LOG_LEVEL_FATAL
// #define LOG_LEVEL LOG_LEVEL_ERROR
// #define LOG_LEVEL LOG_LEVEL_WARNING
// #define LOG_LEVEL LOG_LEVEL_NOTICE
#define LOG_LEVEL LOG_LEVEL_TRACE
// #define LOG_LEVEL LOG_LEVEL_VERBOSE

// Memory optimization for Arduino Uno/Nano: disable debug logs for setting validation
#if defined(BOARD_UNO) || defined(BOARD_NANO)
  #define LOG_FATAL_SETTING false
#else
  #define LOG_FATAL_SETTING true
#endif

#define WATCHDOG true
#define WATCHDOG_INTERVAL 30000

typedef struct SettingBoardPinTrackerLDR {
  uint8_t up;
  uint8_t down;
} SettingBoardPinTrackerLDR;

typedef struct SettingBoardPinMotors {
  uint8_t ena;
  uint8_t in1;
  uint8_t in2;
  uint8_t enb;
  uint8_t in3;
  uint8_t in4;
} SettingBoardPinMotors;

typedef struct SettingBoardPinTrackerCommand {
  uint8_t stop;
} SettingBoardPinTrackerCommand;

typedef struct SettingBoardPinTracker {
  SettingBoardPinTrackerLDR ldr;
  SettingBoardPinMotors motors;
  SettingBoardPinTrackerCommand command;
} SettingBoardPinTracker;

typedef struct SettingBoardPin {
  SettingBoardPinTracker tracker[TRACKER_MAX];
  uint8_t led;
} SettingBoardPin;

typedef struct SettingBoardADC {
  int resolution;
} SettingBoardADC;

typedef struct SettingBoardPWM {
  int resolution;
} SettingBoardPWM;

typedef struct SettingBoardSerial {
  unsigned long baudRate;
} SettingBoardSerial;

typedef struct SettingBoard {
  SettingBoardPin pin;
  SettingBoardADC adc;
  SettingBoardPWM pwm;
  SettingBoardSerial serial;
} SettingBoard;

typedef struct SettingProgramLDR {
  uint16_t threshold;
} SettingProgramLDR;

typedef struct SettingProgramMotor {
  int speed;
} SettingProgramMotor;

typedef struct SettingProgram {
  char* version;
  SettingProgramLDR ldr;
  SettingProgramMotor motor;
} SettingProgram;

typedef struct Setting {
  SettingBoard board;
  SettingProgram program;
} Setting;

void logFatal(const char* message);
void settingInit(Setting *setting);
bool assertSetting(Setting *setting);

#if BOARD_ESP32
void logSetting(Setting *setting);
#endif

#endif
