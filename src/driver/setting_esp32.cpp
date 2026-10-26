#include <Arduino.h>
#include "setting.h"
#ifdef BOARD_ESP32

#define SETTING_BOARD_ADC_RESOLUTION 4095
#define SETTING_BOARD_PWM_RESOLUTION 255
#define SETTING_BOARD_SERIAL_BAUD_RATE 115200
#define SETTING_PROGRAM_TRACKERS_INTERVAL 3 * (60 * 1000)
#define SETTING_PROGRAM_LDR_SAMPLING_INTERVAL 1000
#define SETTING_PROGRAM_LDRS_THRESHOLD 10
#define SETTING_PROGRAM_LDRS_NIGHT_CUTOFF 10
#define SETTING_PROGRAM_MOTORS_SPEED 30
#define SETTING_PROGRAM_MOTORS_TIMEOUT 1000
#define SETTING_PROGRAM_COMMAND_STOP_TIME 1000

Setting* settingInit() {
  Setting *setting = getSetting();
  setting->board.adc.resolution = SETTING_BOARD_ADC_RESOLUTION;
  setting->board.pwm.resolution = SETTING_BOARD_PWM_RESOLUTION;
  setting->board.serial.baudRate = SETTING_BOARD_SERIAL_BAUD_RATE;
  setting->board.pin.tracker[TRACKER_1].ldr.up = 32;
  setting->board.pin.tracker[TRACKER_1].ldr.down = 33;
  setting->board.pin.tracker[TRACKER_1].motors.ena = 25;
  setting->board.pin.tracker[TRACKER_1].motors.in1 = 26;
  setting->board.pin.tracker[TRACKER_1].motors.in2 = 27;
  setting->board.pin.tracker[TRACKER_1].motors.enb = 14;
  setting->board.pin.tracker[TRACKER_1].motors.in3 = 12;
  setting->board.pin.tracker[TRACKER_1].motors.in4 = 13;
  setting->board.pin.tracker[TRACKER_1].command.deploy = 25;
  setting->board.pin.tracker[TRACKER_1].command.retract = 26;
  setting->board.pin.tracker[TRACKER_1].led = 4; // LED_BUILTIN
  setting->board.pin.led = 2; // LED_BUILTIN
  setting->board.pin.driver.command.stop = 27;
  setting->board.pin.mode.manual = 32; // TODO remove
  setting->program.version = VERSION;
  setting->program.command.stop.time = SETTING_PROGRAM_COMMAND_STOP_TIME;
  setting->program.trackers.interval = SETTING_PROGRAM_TRACKERS_INTERVAL;
  setting->program.ldr.sampling.interval = SETTING_PROGRAM_LDR_SAMPLING_INTERVAL;
  setting->program.ldrs.threshold = SETTING_PROGRAM_LDRS_THRESHOLD;
  setting->program.ldrs.night.cutoff = SETTING_PROGRAM_LDRS_NIGHT_CUTOFF;
  setting->program.motors.speed = SETTING_PROGRAM_MOTORS_SPEED;
  setting->program.motors.timeout = SETTING_PROGRAM_MOTORS_TIMEOUT;
  return setting;
}

#endif