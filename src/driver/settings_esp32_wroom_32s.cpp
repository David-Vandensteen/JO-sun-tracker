#include "settings.h"

#define SETTINGS_BOARD_ADC_RESOLUTION 4095
#define SETTINGS_BOARD_PWM_RESOLUTION 255
#define SETTINGS_BOARD_SERIAL_BAUD_RATE 115200
#define SETTINGS_PROGRAM_LDR_THRESHOLD 10
#define SETTINGS_PROGRAM_MOTOR_SPEED 30

void settingsInit(Settings *settings) {
    settings->board.adc.resolution = SETTINGS_BOARD_ADC_RESOLUTION;
    settings->board.pwm.resolution = SETTINGS_BOARD_PWM_RESOLUTION;
    settings->board.serial.baudRate = SETTINGS_BOARD_SERIAL_BAUD_RATE;
    settings->board.pin.ldr.day.up = 32;
    settings->board.pin.ldr.day.down = 33;
    settings->board.pin.ldr.night = 34;
    settings->board.pin.motors.ena = 25;
    settings->board.pin.motors.in1 = 26;
    settings->board.pin.motors.in2 = 27;
    settings->board.pin.motors.enb = 14;
    settings->board.pin.motors.in3 = 12;
    settings->board.pin.motors.in4 = 13;
    settings->board.pin.button.deploy = 4;
    settings->board.pin.button.retract = 5;
    settings->board.pin.button.automatic = 18;
    settings->board.pin.button.scan = 19;
    settings->board.pin.ledStatus = 2;
    settings->program.version = VERSION;
    settings->program.ldr.threshold = SETTINGS_PROGRAM_LDR_THRESHOLD;
    settings->program.motor.speed = SETTINGS_PROGRAM_MOTOR_SPEED;
}
