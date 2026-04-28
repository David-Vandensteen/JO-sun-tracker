// Arduino Uno pin mapping:
/*
L298N Motor Driver:
│
├── 10: ────────► ENA: (L298N, PWM motor 1)
├── 8: ─────────► IN1: (L298N, motor 1 direction)
├── 9: ─────────► IN2: (L298N, motor 1 direction)
│
├── 13: ─────────► ENB: (L298N, PWM motor 2)
├── 11: ─────────► IN3: (L298N, motor 2 direction)
└── 12: ─────────► IN4: (L298N, motor 2 direction)

Selected tracker button:
│
├── 4: ─────────► SELECTED TRACKER: (select tracker button)

Control buttons - work only in manual mode:
│
├── 2: ─────────► RETRACT: (retract button)
└── 3: ─────────► DEPLOY: (deploy button)

Status LED:
│
├── 6: ─────────► STATUS LED

LDR 1 - sun sensor 1:
|
├── A0: ─────────► LDR 1: (analog input)

LDR 2 - sun sensor 2:
|
├── A1: ─────────► LDR 2: (analog input)

LDR 3 - night sensor:
|
├── A2: ─────────► LDR 3: (analog input)
*/

#include "setting.h"
#ifdef BOARD_UNO

#define SETTING_BOARD_ADC_RESOLUTION 1023
#define SETTING_BOARD_PWM_RESOLUTION 255
#define SETTING_BOARD_SERIAL_BAUD_RATE 115200
#define SETTING_PROGRAM_LDR_THRESHOLD 10
#define SETTING_PROGRAM_MOTOR_SPEED 30

void settingInit(Setting *setting) {
	setting->board.adc.resolution = SETTING_BOARD_ADC_RESOLUTION;
	setting->board.pwm.resolution = SETTING_BOARD_PWM_RESOLUTION;
  setting->board.serial.baudRate = SETTING_BOARD_SERIAL_BAUD_RATE;
  setting->board.pin.tracker[TRACKER_1].ldr.day.up = A0;
  setting->board.pin.tracker[TRACKER_1].ldr.day.down = A1;
  setting->board.pin.tracker[TRACKER_1].ldr.night = A2;
  setting->board.pin.tracker[TRACKER_1].motors.ena = 10;
  setting->board.pin.tracker[TRACKER_1].motors.in1 = 8;
  setting->board.pin.tracker[TRACKER_1].motors.in2 = 9;
  setting->board.pin.tracker[TRACKER_1].motors.enb = 13;
  setting->board.pin.tracker[TRACKER_1].motors.in3 = 11;
  setting->board.pin.tracker[TRACKER_1].motors.in4 = 12;
	setting->board.pin.button.deploy = 2;
	setting->board.pin.button.retract = 3;
	setting->board.pin.button.scan = 5;
  setting->board.pin.button.selectedTracker = 4;
	setting->board.pin.ledStatus = 6;
	setting->program.version = VERSION;
	setting->program.ldr.threshold = SETTING_PROGRAM_LDR_THRESHOLD;
	setting->program.motor.speed = SETTING_PROGRAM_MOTOR_SPEED;
}

#endif
