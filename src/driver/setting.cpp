#include "setting.h"
#include "log.h"

bool isValidSetting(Setting *setting) {
  if (setting == nullptr) {
    LOG_ERROR("Setting pointer is null");
    return false;
  }
  if (setting->board.adc.resolution <= 0) {
    LOG_ERROR("ADC resolution <= 0");
    return false;
  }
  if (setting->board.pwm.resolution <= 0) {
    LOG_ERROR("PWM resolution <= 0");
    return false;
  }
  if (setting->board.serial.baudRate <= 0) {
    LOG_ERROR("Serial baudRate <= 0");
    return false;
  }
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    if (setting->board.pin.tracker[i].ldr.day.up == 0) {
      LOG_ERROR("LDR day.up pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].ldr.day.down == 0) {
      LOG_ERROR("LDR day.down pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].ldr.night == 0) {
      LOG_ERROR("LDR night pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.ena == 0) {
      LOG_ERROR("Motor ENA pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.in1 == 0) {
      LOG_ERROR("Motor IN1 pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.in2 == 0) {
      LOG_ERROR("Motor IN2 pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.enb == 0) {
      LOG_ERROR("Motor ENB pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.in3 == 0) {
      LOG_ERROR("Motor IN3 pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.in4 == 0) {
      LOG_ERROR("Motor IN4 pin == 0");
      return false;
    }
  }
  if (setting->board.pin.button.deploy == 0) {
    LOG_ERROR("Deploy button pin == 0");
    return false;
  }
  if (setting->board.pin.button.retract == 0) {
    LOG_ERROR("Retract button pin == 0");
    return false;
  }
  if (setting->board.pin.button.scan == 0) {
    LOG_ERROR("Scan button pin == 0");
    return false;
  }
  if (setting->board.pin.button.selectedTracker == 0) {
    LOG_ERROR("SelectedTracker button pin == 0");
    return false;
  }
  if (setting->board.pin.ledStatus == 0) {
    LOG_ERROR("LED status pin == 0");
    return false;
  }
  if (setting->program.version == nullptr || setting->program.version[0] == '\0') {
    LOG_ERROR("Program version is null or empty");
    return false;
  }
  if (setting->program.ldr.threshold < 0 || setting->program.ldr.threshold > setting->board.adc.resolution) {
    LOG_ERROR("LDR threshold out of range");
    return false;
  }
  if (setting->program.motor.speed < 0 || setting->program.motor.speed > setting->board.pwm.resolution) {
    LOG_ERROR("Motor speed out of range");
    return false;
  }
  return true;
}
