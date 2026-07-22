#include <ArduinoLog.h>
#include "setting.h"

void logFatal(const char* message) {
  #if LOG_FATAL_SETTING
  Log.fatal(message);
  #endif
}

bool assertSetting(Setting *setting) {
  if (setting == nullptr) {
    logFatal("Setting pointer is null");
    return false;
  }
  if (setting->board.adc.resolution <= 0) {
    logFatal("ADC resolution <= 0");
    return false;
  }
  if (setting->board.pwm.resolution <= 0) {
    logFatal("PWM resolution <= 0");
    return false;
  }
  if (setting->board.serial.baudRate <= 0) {
    logFatal("Serial baudRate <= 0");
    return false;
  }
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    if (setting->board.pin.tracker[i].ldr.up == 0) {
      logFatal("LDR day.up pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].ldr.down == 0) {
      logFatal("LDR day.down pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.ena == 0) {
      logFatal("Motor ENA pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.in1 == 0) {
      logFatal("Motor IN1 pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.in2 == 0) {
      logFatal("Motor IN2 pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.enb == 0) {
      logFatal("Motor ENB pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.in3 == 0) {
      logFatal("Motor IN3 pin == 0");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.in4 == 0) {
      logFatal("Motor IN4 pin == 0");
      return false;
    }
  }
  if (setting->board.pin.command.deploy == 0) {
    logFatal("Deploy button pin == 0");
    return false;
  }
  if (setting->board.pin.command.retract == 0) {
    logFatal("Retract button pin == 0");
    return false;
  }
  if (setting->board.pin.led == 0) {
    logFatal("LED pin == 0");
    return false;
  }
  if (setting->program.version == nullptr || setting->program.version[0] == '\0') {
    logFatal("Program version is null or empty");
    return false;
  }
  if (setting->program.ldr.threshold < 0 || setting->program.ldr.threshold > setting->board.adc.resolution) {
    logFatal("LDR threshold out of range");
    return false;
  }
  if (setting->program.motor.speed < 0 || setting->program.motor.speed > setting->board.pwm.resolution) {
    logFatal("Motor speed out of range");
    return false;
  }

  return true;
}

#ifdef BOARD_ESP32
void logSetting(Setting *setting) {
  Log.traceln("Setting:");
  Log.traceln("- Board:");
  Log.traceln("  - ADC resolution: %d", setting->board.adc.resolution);
  Log.traceln("  - PWM resolution: %d", setting->board.pwm.resolution);
  Log.traceln("  - Serial baud rate: %lu", setting->board.serial.baudRate);
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    Log.traceln("  - Tracker %d:", i);
    Log.traceln("    - LDR day up pin: %d", setting->board.pin.tracker[i].ldr.day.up);
    Log.traceln("    - LDR day down pin: %d", setting->board.pin.tracker[i].ldr.day.down);
    Log.traceln("    - Motor ENA pin: %d", setting->board.pin.tracker[i].motors.ena);
    Log.traceln("    - Motor IN1 pin: %d", setting->board.pin.tracker[i].motors.in1);
    Log.traceln("    - Motor IN2 pin: %d", setting->board.pin.tracker[i].motors.in2);
    Log.traceln("    - Motor ENB pin: %d", setting->board.pin.tracker[i].motors.enb);
    Log.traceln("    - Motor IN3 pin: %d", setting->board.pin.tracker[i].motors.in3);
    Log.traceln("    - Motor IN4 pin: %d", setting->board.pin.tracker[i].motors.in4);
  }
  Log.traceln("  - Command deploy pin: %d", setting->board.pin.command.deploy);
  Log.traceln("  - Command retract pin: %d", setting->board.pin.command.retract);
  Log.traceln("  - LED status pin: %d", setting->board.pin.ledStatus);
  Log.traceln("- Program:");
  Log.traceln("  - Version: %s", setting->program.version);
  Log.traceln("  - LDR threshold: %d", setting->program.ldr.threshold);
  Log.traceln("  - Motor speed: %d", setting->program.motor.speed);
}
#endif