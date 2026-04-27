#ifndef SETTINGS_H
#define SETTINGS_H
#include <Arduino.h>

#define TRUE 1
#define FALSE 0
#define DEBUG TRUE

#define VERSION "0.0.1"

// #define BOARD_UNO
#define BOARD_ESP32_WROOM_32S

#define TRACKER_MAX 1
#define TRACKER_1 0

typedef struct SettingsBoardPinTrackerLDRDay {
  uint8_t up;
  uint8_t down;
} SettingsBoardPinTrackerLDRDay;

typedef uint8_t SettingsBoardPinTrackerLDRNight;

typedef struct SettingsBoardPinTrackerLDR {
  SettingsBoardPinTrackerLDRDay day;
  SettingsBoardPinTrackerLDRNight night;
} SettingsBoardPinTrackerLDR;

typedef struct SettingsBoardPinButton {
  uint8_t deploy;
  uint8_t retract;
  uint8_t automatic;
  uint8_t scan;

} SettingsBoardPinButton;

typedef struct SettingsBoardPinMotors {
  uint8_t ena;
  uint8_t in1;
  uint8_t in2;
  uint8_t enb;
  uint8_t in3;
  uint8_t in4;
} SettingsBoardPinMotor;

typedef struct SettingsBoardPinTracker {
  SettingsBoardPinTrackerLDR ldr;
  SettingsBoardPinMotor motors;
} SettingsBoardPinTracker;

typedef struct SettingsBoardPin {
  SettingsBoardPinTracker tracker[TRACKER_MAX];
  SettingsBoardPinButton button;
  uint8_t ledStatus;

} SettingsBoardPin;

typedef struct SettingsBoardADC {
  int resolution;
} SettingsBoardADC;

typedef struct SettingsBoardPWM {
  int resolution;
} SettingsBoardPWM;

typedef struct SettingBoardSerial {
  unsigned long baudRate;
} SettingsBoardSerial;

typedef struct SettingsBoard {
  SettingsBoardPin pin;
  SettingsBoardADC adc;
  SettingsBoardPWM pwm;
  SettingsBoardSerial serial;
} SettingsBoard;

typedef struct SettingsProgramLDR {
  uint16_t threshold;
} SettingsProgramLDR;

typedef struct SettingsProgramMotor {
  int speed;
} SettingsProgramMotor;

typedef struct SettingsProgram {
  char* version;
  SettingsProgramLDR ldr;
  SettingsProgramMotor motor;
} SettingsProgram;

typedef struct Settings {
  SettingsBoard board;
  SettingsProgram program;
} Settings;

void settingsInit(Settings *settings);
bool isValidSettings(Settings *settings);

#endif