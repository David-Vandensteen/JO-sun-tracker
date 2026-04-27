#include "setting.h"

bool isValidSetting(Setting *setting) {
  if (setting == nullptr) return false;
  if (setting->board.adc.resolution <= 0) return false;
  if (setting->board.pwm.resolution <= 0) return false;
  if (setting->board.serial.baudRate <= 0) return false;
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    if (setting->board.pin.tracker[i].ldr.day.up == 0) return false;
    if (setting->board.pin.tracker[i].ldr.day.down == 0) return false;
    if (setting->board.pin.tracker[i].ldr.night == 0) return false;
    if (setting->board.pin.tracker[i].motors.ena == 0) return false;
    if (setting->board.pin.tracker[i].motors.in1 == 0) return false;
    if (setting->board.pin.tracker[i].motors.in2 == 0) return false;
    if (setting->board.pin.tracker[i].motors.enb == 0) return false;
    if (setting->board.pin.tracker[i].motors.in3 == 0) return false;
    if (setting->board.pin.tracker[i].motors.in4 == 0) return false;
  }
  if (setting->board.pin.button.deploy == 0) return false;
  if (setting->board.pin.button.retract == 0) return false;
  if (setting->board.pin.button.automatic == 0) return false;
  if (setting->board.pin.button.scan == 0) return false;
  if (setting->program.version == nullptr || setting->program.version[0] == '\0') return false;
  if (setting->program.ldr.threshold < 0 || setting->program.ldr.threshold > setting->board.adc.resolution) return false;
  if (setting->program.motor.speed < 0 || setting->program.motor.speed > setting->board.pwm.resolution) return false;

  return true;
}
