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

Mode button:
│
├── 4: ─────────► AUTO: (mode auto/manual button)

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

#include "settings.h"
#ifdef BOARD_UNO

#define SETTINGS_BOARD_ADC_RESOLUTION 1023
#define SETTINGS_BOARD_PWM_RESOLUTION 255
#define SETTINGS_BOARD_SERIAL_BAUD_RATE 9600
#define SETTINGS_PROGRAM_LDR_THRESHOLD 10
#define SETTINGS_PROGRAM_MOTOR_SPEED 30

void settingsInit(Settings *settings) {
	settings->board.adc.resolution = SETTINGS_BOARD_ADC_RESOLUTION;
	settings->board.pwm.resolution = SETTINGS_BOARD_PWM_RESOLUTION;
  settings->board.serial.baudRate = SETTINGS_BOARD_SERIAL_BAUD_RATE;
  settings->board.pin.tracker[TRACKER_1].ldr.day.up = A0;
  settings->board.pin.tracker[TRACKER_1].ldr.day.down = A1;
  settings->board.pin.tracker[TRACKER_1].ldr.night = A2;
  settings->board.pin.tracker[TRACKER_1].motors.ena = 10;
  settings->board.pin.tracker[TRACKER_1].motors.in1 = 8;
  settings->board.pin.tracker[TRACKER_1].motors.in2 = 9;
  settings->board.pin.tracker[TRACKER_1].motors.enb = 13;
  settings->board.pin.tracker[TRACKER_1].motors.in3 = 11;
  settings->board.pin.tracker[TRACKER_1].motors.in4 = 12;
	settings->board.pin.button.deploy = 2;
	settings->board.pin.button.retract = 3;
	settings->board.pin.button.automatic = 4;
	settings->board.pin.button.scan = 5;
	settings->board.pin.ledStatus = 6;
	settings->program.version = VERSION;
	settings->program.ldr.threshold = SETTINGS_PROGRAM_LDR_THRESHOLD;
	settings->program.motor.speed = SETTINGS_PROGRAM_MOTOR_SPEED;
}

#endif
