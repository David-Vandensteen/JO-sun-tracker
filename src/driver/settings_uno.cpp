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
├── 4: ─────────► AUTO: (mode auto/manual switch button)

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

const Settings settings = {
  .pin = {
    .LDR = {
      .day = {
        .up = A0,
        .down = A1
      },
      .night = A2
    },
    .motors = {
      .ena = 10,
      .in1 = 8,
      .in2 = 9,
      .enb = 13,
      .in3 = 11,
      .in4 = 12
    },
    .button = {
      .deploy = 2,
      .retract = 3,
      .automatic = 4,
      .scan = 5
    },
    .LEDStatus = 6
  },
  .program = {
    .version = VERSION,
    .LDR = {
      .threshold = SETTINGS_PROGRAM_LDR_THRESHOLD
    },
    .motor = {
      .speed = SETTINGS_PROGRAM_MOTOR_SPEED
    }
  }
};
