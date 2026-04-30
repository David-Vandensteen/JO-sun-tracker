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
  Log.trace("Setting:\n");
  Log.trace("- Board:\n");
  Log.trace("  - ADC resolution: %d\n", setting->board.adc.resolution);
  Log.trace("  - PWM resolution: %d\n", setting->board.pwm.resolution);
  Log.trace("  - Serial baud rate: %lu\n", setting->board.serial.baudRate);
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    Log.trace("  - Tracker %d:\n", i);
    Log.trace("    - LDR day up pin: %d\n", setting->board.pin.tracker[i].ldr.day.up);
    Log.trace("    - LDR day down pin: %d\n", setting->board.pin.tracker[i].ldr.day.down);
    Log.trace("    - LDR back pin: %d\n", setting->board.pin.tracker[i].ldr.back);
    Log.trace("    - Motor ENA pin: %d\n", setting->board.pin.tracker[i].motors.ena);
    Log.trace("    - Motor IN1 pin: %d\n", setting->board.pin.tracker[i].motors.in1);
    Log.trace("    - Motor IN2 pin: %d\n", setting->board.pin.tracker[i].motors.in2);
    Log.trace("    - Motor ENB pin: %d\n", setting->board.pin.tracker[i].motors.enb);
    Log.trace("    - Motor IN3 pin: %d\n", setting->board.pin.tracker[i].motors.in3);
    Log.trace("    - Motor IN4 pin: %d\n", setting->board.pin.tracker[i].motors.in4);
  }
  Log.trace("  - Button deploy pin: %d\n", setting->board.pin.button.deploy);
  Log.trace("  - Button retract pin: %d\n", setting->board.pin.button.retract);
  Log.trace("  - Button scan pin: %d\n", setting->board.pin.button.scan);
  Log.trace("  - Button selectedTracker pin: %d\n", setting->board.pin.button.selectedTracker);
  Log.trace("  - LED status pin: %d\n", setting->board.pin.ledStatus);
  Log.trace("- Program:\n");
  Log.trace("  - Version: %s\n", setting->program.version);
  Log.trace("  - LDR threshold: %d\n", setting->program.ldr.threshold);
  Log.trace("  - Motor speed: %d\n", setting->program.motor.speed);
}
#endif