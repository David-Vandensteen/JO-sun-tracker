#ifndef SETTING_H
#define SETTING_H
#include <Arduino.h>
#include <ArduinoLog.h>

#define VERSION "0.0.1"

#define TRACKER_MAX 1
#define TRACKER_1 0

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

#define LOG_FATAL_SETTING true

#define LOG_LDR_INTERVAL true
#define LOG_TRACKER_INTERVAL true

#define HEARTBEAT true
#define HEARTBEAT_INTERVAL 30000

// ------------------
// FEAT
// ------------------

#define FEAT_DISPLAY true
#define FEAT_WIFI true
#define FEAT_WEBSERVER true

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
  uint8_t mode;
} SettingBoardPinTrackerCommand;

typedef struct SettingBoardPinTracker {
  SettingBoardPinTrackerLDR ldr;
  SettingBoardPinMotors motors;
  SettingBoardPinTrackerCommand command;
  uint8_t led;
} SettingBoardPinTracker;

typedef struct SettingBoardPinDriverCommand {
  uint8_t stop;
} SettingBoardPinDriverCommand;

typedef struct SettingBoardPinDriver {
  SettingBoardPinDriverCommand command;
} SettingBoardPinDriver;

typedef struct SettingBoardPinDisplay {
  uint8_t sda;
  uint8_t scl;
} SettingBoardPinDisplay;

typedef struct SettingBoardPin {
  SettingBoardPinTracker tracker[TRACKER_MAX];
  SettingBoardPinDriver driver;
  SettingBoardPinDisplay display;
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

typedef struct SettingProgramTrackersCommand {
  unsigned long debounce;
} SettingProgramTrackersCommand;

typedef struct SettingProgramTrackers {
  unsigned long interval;
  SettingProgramTrackersCommand command;
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

typedef struct SettingProgramCommandStop {
  unsigned int pressMs;
} SettingProgramCommandStop;

typedef struct SettingProgramCommand {
  SettingProgramCommandStop stop;
} SettingProgramCommand;

typedef struct SettingProgramDisplay {
  uint8_t address;
  uint8_t width;
  uint8_t height;
} SettingProgramDisplay;

typedef struct SettingProgram {
  char *version;
  SettingProgramTrackers trackers;
  SettingProgramLDR ldr;
  SettingProgramLDRs ldrs;
  SettingProgramMotors motors;
  SettingProgramCommand command;
  SettingProgramDisplay display;
} SettingProgram;

typedef struct SettingSecretWifi {
  const char *ssid;
  const char *password;
} SettingSecretWifi;

typedef struct SettingSecret {
  SettingSecretWifi wifi;
} SettingSecret;

typedef struct Setting {
  SettingBoard board;
  SettingProgram program;
  SettingSecret secret;
} Setting;

void logFatal(const char *message);
Setting* settingInit();
bool assertSetting();
Setting* getSetting();
void logSetting();

#endif
