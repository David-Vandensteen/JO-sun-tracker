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

#ifdef BOARD_ESP32_WROOM_32S
void logSetting(Setting *setting) {
  LOG_DEBUG("Setting:");
  LOG_DEBUG("- Board:");
  LOG_DEBUG("  - ADC resolution: " + String(setting->board.adc.resolution));
  LOG_DEBUG("  - PWM resolution: " + String(setting->board.pwm.resolution));
  LOG_DEBUG("  - Serial baud rate: " + String(setting->board.serial.baudRate));
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    LOG_DEBUG("  - Tracker " + String(i) + ":");
    LOG_DEBUG("    - LDR day up pin: " + String(setting->board.pin.tracker[i].ldr.day.up));
    LOG_DEBUG("    - LDR day down pin: " + String(setting->board.pin.tracker[i].ldr.day.down));
    LOG_DEBUG("    - LDR night pin: " + String(setting->board.pin.tracker[i].ldr.night));
    LOG_DEBUG("    - Motor ENA pin: " + String(setting->board.pin.tracker[i].motors.ena));
    LOG_DEBUG("    - Motor IN1 pin: " + String(setting->board.pin.tracker[i].motors.in1));
    LOG_DEBUG("    - Motor IN2 pin: " + String(setting->board.pin.tracker[i].motors.in2));
    LOG_DEBUG("    - Motor ENB pin: " + String(setting->board.pin.tracker[i].motors.enb));
    LOG_DEBUG("    - Motor IN3 pin: " + String(setting->board.pin.tracker[i].motors.in3));
    LOG_DEBUG("    - Motor IN4 pin: " + String(setting->board.pin.tracker[i].motors.in4));
  }
  LOG_DEBUG("  - Button deploy pin: " + String(setting->board.pin.button.deploy));
  LOG_DEBUG("  - Button retract pin: " + String(setting->board.pin.button.retract));
  LOG_DEBUG("  - Button scan pin: " + String(setting->board.pin.button.scan));
  LOG_DEBUG("  - Button selectedTracker pin: " + String(setting->board.pin.button.selectedTracker));
  LOG_DEBUG("  - LED status pin: " + String(setting->board.pin.ledStatus));
  LOG_DEBUG("- Program:");
  LOG_DEBUG("  - Version: " + String(setting->program.version));
  LOG_DEBUG("  - LDR threshold: " + String(setting->program.ldr.threshold));
  LOG_DEBUG("  - Motor speed: " + String(setting->program.motor.speed));
}
#endif