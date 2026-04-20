#ifndef SETTINGS_H
#define SETTINGS_H
#include <Arduino.h>

#define VERSION "0.0.1"

#define SETTINGS_PROGRAM_LDR_THRESHOLD 10
#define SETTINGS_PROGRAM_MOTOR_SPEED 30

#define ANALOG_RESOLUTION 1023
#define PWM_RESOLUTION 255

typedef struct SettingsPinLDRDay {
  const uint8_t up;
  const uint8_t down;
} SettingsPinLDRDay;

typedef uint8_t SettingsPinLDRNight;

typedef struct SettingsPinLDR {
  const SettingsPinLDRDay day;
  const SettingsPinLDRNight night;
} SettingsPinLDR;

typedef struct SettingsPinButton {
  const uint8_t deploy;
  const uint8_t retract;
  const uint8_t automatic;
  const uint8_t scan;

} SettingsPinButton;

typedef struct SettingsPinMotors {
  const uint8_t ena;
  const uint8_t in1;
  const uint8_t in2;
  const uint8_t enb;
  const uint8_t in3;
  const uint8_t in4;
} SettingsPinMotor;

typedef struct SettingsPin {
  const SettingsPinLDR LDR;
  const SettingsPinMotor motors;
  const SettingsPinButton button;

} SettingsPin;

typedef struct SettingsProgramLDR {
  const uint16_t threshold;
} SettingsProgramLDR;

typedef struct SettingsProgramMotor {
  const int speed;
} SettingsProgramMotor;

typedef struct SettingsProgram {
  const char* version;
  const SettingsProgramLDR LDR;
  const SettingsProgramMotor motor;
} SettingsProgram;


typedef struct Settings {
  const SettingsPin pin;
  const SettingsProgram program;
} Settings;

extern const Settings settings;

#endif