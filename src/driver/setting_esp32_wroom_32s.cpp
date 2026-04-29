#include "setting.h"
#ifdef BOARD_ESP32_WROOM_32S

#define SETTING_BOARD_ADC_RESOLUTION 4095
#define SETTING_BOARD_PWM_RESOLUTION 255
#define SETTING_BOARD_SERIAL_BAUD_RATE 115200
#define SETTING_PROGRAM_LDR_THRESHOLD 10
#define SETTING_PROGRAM_MOTOR_SPEED 30

void settingInit(Setting *setting) {
    setting->board.adc.resolution = SETTING_BOARD_ADC_RESOLUTION;
    setting->board.pwm.resolution = SETTING_BOARD_PWM_RESOLUTION;
    setting->board.serial.baudRate = SETTING_BOARD_SERIAL_BAUD_RATE;
    setting->board.pin.tracker[TRACKER_1].ldr.day.up = 32;
    setting->board.pin.tracker[TRACKER_1].ldr.day.down = 33;
    setting->board.pin.tracker[TRACKER_1].ldr.back = 34;
    setting->board.pin.tracker[TRACKER_1].motors.ena = 25;
    setting->board.pin.tracker[TRACKER_1].motors.in1 = 26;
    setting->board.pin.tracker[TRACKER_1].motors.in2 = 27;
    setting->board.pin.tracker[TRACKER_1].motors.enb = 14;
    setting->board.pin.tracker[TRACKER_1].motors.in3 = 12;
    setting->board.pin.tracker[TRACKER_1].motors.in4 = 13;
    setting->board.pin.button.deploy = 4;
    setting->board.pin.button.retract = 5;
    setting->board.pin.button.scan = 19;
    setting->board.pin.button.selectedTracker = 20;
    setting->board.pin.ledStatus = 2;
    setting->program.version = VERSION;
    setting->program.ldr.threshold = SETTING_PROGRAM_LDR_THRESHOLD;
    setting->program.motor.speed = SETTING_PROGRAM_MOTOR_SPEED;
}

#endif