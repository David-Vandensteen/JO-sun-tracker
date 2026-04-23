#ifndef SETTINGS_H
#define SETTINGS_H
#include <Arduino.h>

#define VERSION "0.0.1"
#define BOARD_UNO

typedef struct SettingsBoardPinLDRDay {
  uint8_t up;
  uint8_t down;
} SettingsBoardPinLDRDay;

typedef uint8_t SettingsBoardPinLDRNight;

typedef struct SettingsBoardPinLDR {
  SettingsBoardPinLDRDay day;
  SettingsBoardPinLDRNight night;
} SettingsBoardPinLDR;

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

typedef struct SettingsBoardPin {
  SettingsBoardPinLDR ldr;
  SettingsBoardPinMotor motors;
  SettingsBoardPinButton button;
  uint8_t ledStatus;

} SettingsBoardPin;

typedef struct SettingsBoardADC {
  int resolution;
} SettingsBoardADC;

typedef struct SettingsBoardPWM {
  int resolution;
} SettingsBoardPWM;

typedef struct SettingsBoard {
  SettingsBoardPin pin;
  SettingsBoardADC adc;
  SettingsBoardPWM pwm;
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

#endif