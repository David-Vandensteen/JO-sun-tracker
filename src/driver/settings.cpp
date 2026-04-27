#include "settings.h"

bool isValidSettings(Settings *settings) {
  if (settings == nullptr) return false;
  if (settings->board.adc.resolution <= 0) return false;
  if (settings->board.pwm.resolution <= 0) return false;
  if (settings->board.serial.baudRate <= 0) return false;
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    if (settings->board.pin.tracker[i].ldr.day.up == 0) return false;
    if (settings->board.pin.tracker[i].ldr.day.down == 0) return false;
    if (settings->board.pin.tracker[i].ldr.night == 0) return false;
    if (settings->board.pin.tracker[i].motors.ena == 0) return false;
    if (settings->board.pin.tracker[i].motors.in1 == 0) return false;
    if (settings->board.pin.tracker[i].motors.in2 == 0) return false;
    if (settings->board.pin.tracker[i].motors.enb == 0) return false;
    if (settings->board.pin.tracker[i].motors.in3 == 0) return false;
    if (settings->board.pin.tracker[i].motors.in4 == 0) return false;
  }
  if (settings->board.pin.button.deploy == 0) return false;
  if (settings->board.pin.button.retract == 0) return false;
  if (settings->board.pin.button.automatic == 0) return false;
  if (settings->program.version == nullptr || settings->program.version[0] == '\0') return false;
  if (settings->program.ldr.threshold < 0 || settings->program.ldr.threshold > settings->board.adc.resolution) return false;
  if (settings->program.motor.speed < 0 || settings->program.motor.speed > settings->board.pwm.resolution) return false;

  return true;
}
