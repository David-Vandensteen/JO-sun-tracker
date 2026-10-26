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
// #define BOARD_UNO
#define BOARD_ESP32

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

#define LOG_LDR_INTERVAL true
#define LOG_TRACKER_INTERVAL true

#define HEARTBEAT true
#define HEARTBEAT_INTERVAL 30000

// ------------------
//  MOCK
// ------------------
#define MOCK_LDR_AVERAGE_PERCENT_VALUE false

// --------------------------------------------------------
// Setting structures for board and program configuration
// --------------------------------------------------------
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
  uint8_t deploy;
  uint8_t retract;
} SettingBoardPinTrackerCommand;

typedef struct SettingBoardPinTracker {
  SettingBoardPinTrackerLDR ldr;
  SettingBoardPinMotors motors;
  SettingBoardPinTrackerCommand command;
  uint8_t led;
} SettingBoardPinTracker;

typedef struct SettingBoardPinMode {
  uint8_t manual;
} SettingBoardPinMode;

typedef struct SettingBoardPinDriverCommand {
  uint8_t stop;
} SettingBoardPinDriverCommand;

typedef struct SettingBoardPinDriver {
  SettingBoardPinDriverCommand command;
} SettingBoardPinDriver;

typedef struct SettingBoardPin {
  SettingBoardPinTracker tracker[TRACKER_MAX];
  SettingBoardPinMode mode;
  SettingBoardPinDriver driver;
  uint8_t led;

} SettingBoardPin;

typedef struct SettingBoardADC {
  uint16_t resolution;
} SettingBoardADC;

typedef struct SettingBoardPWM {
  uint8_t resolution;
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

typedef struct SettingProgramTrackers {
  unsigned long interval;
} SettingProgramTrackers;

typedef struct SettingProgramLDRSampling {
  unsigned long interval;
} SettingProgramLDRSampling;

typedef struct SettingProgramLDRsNight {
  uint8_t cutoff;
} SettingProgramLDRsNight;

typedef struct SettingProgramLDRs {
  uint8_t threshold;
  SettingProgramLDRsNight night;
} SettingProgramLDRs;

typedef struct SettingProgramLDR {
  SettingProgramLDRSampling sampling;
} SettingProgramLDR;

typedef struct SettingProgramMotors {
  unsigned long timeout;
  uint8_t speed;
} SettingProgramMotors;

typedef struct SettingProgram {
  char *version;
  SettingProgramTrackers trackers;
  SettingProgramLDR ldr;
  SettingProgramLDRs ldrs;
  SettingProgramMotors motors;
} SettingProgram;

typedef struct Setting {
  SettingBoard board;
  SettingProgram program;
} Setting;

void logFatal(const char *message);
Setting* settingInit();
bool assertSetting();
Setting* getSetting();
#ifdef BOARD_ESP32
void logSetting();
#endif

#endif
