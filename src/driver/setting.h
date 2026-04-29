#ifndef SETTING_H
#define SETTING_H
#include <Arduino.h>

#define VERSION "0.0.1"

#define DEBUG true

// #define BOARD_NANO
#define BOARD_UNO
// #define BOARD_ESP32_WROOM_32S

#define TRACKER_MAX 1
#define TRACKER_1 0

#ifdef BOARD_UNO
#undef TRACKER_MAX
#define TRACKER_MAX 1
#endif

#ifdef BOARD_NANO
#undef TRACKER_MAX
#define TRACKER_MAX 1
#endif

typedef struct SettingBoardPinTrackerLDRDay {
  uint8_t up;
  uint8_t down;
} SettingBoardPinTrackerLDRDay;

typedef uint8_t SettingBoardPinTrackerLDRNight;

typedef struct SettingBoardPinTrackerLDR {
  SettingBoardPinTrackerLDRDay day;
  SettingBoardPinTrackerLDRNight night;
} SettingBoardPinTrackerLDR;

typedef struct SettingBoardPinButton {
  uint8_t deploy;
  uint8_t retract;
  uint8_t scan;
  uint8_t selectedTracker;
} SettingBoardPinButton;

typedef struct SettingBoardPinMotors {
  uint8_t ena;
  uint8_t in1;
  uint8_t in2;
  uint8_t enb;
  uint8_t in3;
  uint8_t in4;
} SettingBoardPinMotors;

typedef struct SettingBoardPinTracker {
  SettingBoardPinTrackerLDR ldr;
  SettingBoardPinMotors motors;
} SettingBoardPinTracker;

typedef struct SettingBoardPin {
  SettingBoardPinTracker tracker[TRACKER_MAX];
  SettingBoardPinButton button;
  uint8_t ledStatus;
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

void settingInit(Setting *setting);
bool isValidSetting(Setting *setting);

#if BOARD_ESP32_WROOM_32S
void logSetting(Setting *setting);
#endif

#endif
