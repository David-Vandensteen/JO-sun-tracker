#include <ArduinoLog.h>
#include "setting.h"

bool isValidSetting(Setting *setting) {
  if (setting == nullptr) {
    Log.fatal("Setting pointer is null");
    return false;
  }
  if (setting->board.adc.resolution <= 0) {
    Log.fatal("ADC resolution <= 0");
    return false;
  }
  if (setting->board.pwm.resolution <= 0) {
    Log.fatal("PWM resolution <= 0");
    return false;
  }
  if (setting->board.serial.baudRate <= 0) {
    Log.fatal("Serial baudRate <= 0");
    return false;
  }
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    if (setting->board.pin.tracker[i].ldr.day.up == 0) {
      Log.fatal("LDR day.up pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].ldr.day.down == 0) {
      Log.fatal("LDR day.down pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].ldr.back == 0) {
      Log.fatal("LDR back pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.ena == 0) {
      Log.fatal("Motor ENA pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.in1 == 0) {
      Log.fatal("Motor IN1 pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.in2 == 0) {
      Log.fatal("Motor IN2 pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.enb == 0) {
      Log.fatal("Motor ENB pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.in3 == 0) {
      Log.fatal("Motor IN3 pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.in4 == 0) {
      Log.fatal("Motor IN4 pin == 0");
      return false;
    }
  }
  if (setting->board.pin.button.deploy == 0) {
    Log.fatal("Deploy button pin == 0");
    return false;
  }
  if (setting->board.pin.button.retract == 0) {
    Log.fatal("Retract button pin == 0");
    return false;
  }
  if (setting->board.pin.button.scan == 0) {
    Log.fatal("Scan button pin == 0");
    return false;
  }
  if (setting->board.pin.button.selectedTracker == 0) {
    Log.fatal("SelectedTracker button pin == 0");
    return false;
  }
  if (setting->board.pin.ledStatus == 0) {
    Log.fatal("LED status pin == 0");
    return false;
  }
  if (setting->program.version == nullptr || setting->program.version[0] == '\0') {
    Log.fatal("Program version is null or empty");
    return false;
  }
  if (setting->program.ldr.threshold < 0 || setting->program.ldr.threshold > setting->board.adc.resolution) {
    Log.fatal("LDR threshold out of range");
    return false;
  }
  if (setting->program.motor.speed < 0 || setting->program.motor.speed > setting->board.pwm.resolution) {
    Log.fatal("Motor speed out of range");
    return false;
  }

  return true;
}

#ifdef BOARD_ESP32_WROOM_32S
void logSetting(Setting *setting) {
  LOG_DEBUG("Setting:");
  LOG_DEBUG("- Board:");
  LOG_DEBUGF("  - ADC resolution: %d", setting->board.adc.resolution);
  LOG_DEBUGF("  - PWM resolution: %d", setting->board.pwm.resolution);
  LOG_DEBUGF("  - Serial baud rate: %lu", setting->board.serial.baudRate);
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    LOG_DEBUGF("  - Tracker %d:", i);
    LOG_DEBUGF("    - LDR day up pin: %d", setting->board.pin.tracker[i].ldr.day.up);
    LOG_DEBUGF("    - LDR day down pin: %d", setting->board.pin.tracker[i].ldr.day.down);
    LOG_DEBUGF("    - LDR back pin: %d", setting->board.pin.tracker[i].ldr.back);
    LOG_DEBUGF("    - Motor ENA pin: %d", setting->board.pin.tracker[i].motors.ena);
    LOG_DEBUGF("    - Motor IN1 pin: %d", setting->board.pin.tracker[i].motors.in1);
    LOG_DEBUGF("    - Motor IN2 pin: %d", setting->board.pin.tracker[i].motors.in2);
    LOG_DEBUGF("    - Motor ENB pin: %d", setting->board.pin.tracker[i].motors.enb);
    LOG_DEBUGF("    - Motor IN3 pin: %d", setting->board.pin.tracker[i].motors.in3);
    LOG_DEBUGF("    - Motor IN4 pin: %d", setting->board.pin.tracker[i].motors.in4);
  }
  LOG_DEBUGF("  - Button deploy pin: %d", setting->board.pin.button.deploy);
  LOG_DEBUGF("  - Button retract pin: %d", setting->board.pin.button.retract);
  LOG_DEBUGF("  - Button scan pin: %d", setting->board.pin.button.scan);
  LOG_DEBUGF("  - Button selectedTracker pin: %d", setting->board.pin.button.selectedTracker);
  LOG_DEBUGF("  - LED status pin: %d", setting->board.pin.ledStatus);
  LOG_DEBUG("- Program:");
  LOG_DEBUGF("  - Version: %s", setting->program.version);
  LOG_DEBUGF("  - LDR threshold: %d", setting->program.ldr.threshold);
  LOG_DEBUGF("  - Motor speed: %d", setting->program.motor.speed);
}
#endif