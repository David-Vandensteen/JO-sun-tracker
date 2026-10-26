#include <ArduinoLog.h>
#include "setting.h"

static Setting setting;

void logFatal(const char* message) {
  #if LOG_FATAL_SETTING
  Log.fatal(message);
  #endif
}

// Returns false when the pin is already registered by another setting
static bool registerPin(uint8_t pin, const char *name, uint8_t *pins, const char **names, uint8_t &count) {
  for (uint8_t i = 0; i < count; i++) {
    if (pins[i] == pin) {
      #if LOG_FATAL_SETTING
      Log.fatal("Pin %d used by both '%s' and '%s'", pin, names[i], name);
      #endif
      return false;
    }
  }
  pins[count] = pin;
  names[count] = name;
  count++;
  return true;
}

static bool assertPinsUnique(Setting *setting) {
  uint8_t pins[TRACKER_MAX * 11 + 3];
  const char *names[TRACKER_MAX * 11 + 3];
  uint8_t count = 0;
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    SettingBoardPinTracker &pin = setting->board.pin.tracker[i];
    if (!registerPin(pin.ldr.up, "LDR up", pins, names, count)) return false;
    if (!registerPin(pin.ldr.down, "LDR down", pins, names, count)) return false;
    if (!registerPin(pin.motors.ena, "Motor ENA", pins, names, count)) return false;
    if (!registerPin(pin.motors.in1, "Motor IN1", pins, names, count)) return false;
    if (!registerPin(pin.motors.in2, "Motor IN2", pins, names, count)) return false;
    if (!registerPin(pin.motors.enb, "Motor ENB", pins, names, count)) return false;
    if (!registerPin(pin.motors.in3, "Motor IN3", pins, names, count)) return false;
    if (!registerPin(pin.motors.in4, "Motor IN4", pins, names, count)) return false;
    if (!registerPin(pin.command.deploy, "Tracker deploy command", pins, names, count)) return false;
    if (!registerPin(pin.command.retract, "Tracker retract command", pins, names, count)) return false;
    if (!registerPin(pin.led, "Tracker LED", pins, names, count)) return false;
  }
  if (!registerPin(setting->board.pin.led, "LED status", pins, names, count)) return false;
  if (!registerPin(setting->board.pin.driver.command.stop, "Stop command", pins, names, count)) return false;
  if (!registerPin(setting->board.pin.mode.manual, "Manual mode", pins, names, count)) return false;
  return true;
}

bool assertSetting() {
  Setting *setting = getSetting();
  if (setting == nullptr) {
    logFatal("Setting pointer is null");
    return false;
  }
  if (setting->board.adc.resolution == 0) {
    logFatal("ADC resolution is invalid");
    return false;
  }
  if (setting->board.pwm.resolution == 0) {
    logFatal("PWM resolution is invalid");
    return false;
  }
  if (setting->board.serial.baudRate == 0) {
    logFatal("Serial baudRate is invalid");
    return false;
  }
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    if (setting->board.pin.tracker[i].ldr.up == 0) {
      logFatal("LDR up is invalid");
      return false;
    }
    if (setting->board.pin.tracker[i].ldr.down == 0) {
      logFatal("LDR down is invalid");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.ena == 0) {
      logFatal("Motor ENA pin is invalid");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.in1 == 0) {
      logFatal("Motor IN1 pin is invalid");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.in2 == 0) {
      logFatal("Motor IN2 pin is invalid");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.enb == 0) {
      logFatal("Motor ENB pin is invalid");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.in3 == 0) {
      logFatal("Motor IN3 pin is invalid");
      return false;
    }
    if (setting->board.pin.tracker[i].motors.in4 == 0) {
      logFatal("Motor IN4 pin is invalid");
      return false;
    }
    if (setting->board.pin.tracker[i].command.deploy == 0) {
      logFatal("Tracker deploy command pin is invalid");
      return false;
    }
    if (setting->board.pin.tracker[i].command.retract == 0) {
      logFatal("Tracker retract command pin is invalid");
      return false;
    }
    if (setting->board.pin.tracker[i].led == 0) {
      logFatal("Tracker LED pin is invalid");
      return false;
    }
  }
  if (setting->board.pin.led == 0) {
    logFatal("LED pin is invalid");
    return false;
  }
  if (setting->board.pin.driver.command.stop == 0) {
    logFatal("Stop command pin is invalid");
    return false;
  }
  if (setting->board.pin.mode.manual == 0) {
    logFatal("Manual mode pin is invalid");
    return false;
  }
  if (!assertPinsUnique(setting)) {
    logFatal("Duplicate GPIO pin in board settings");
    return false;
  }
  if (setting->program.version == nullptr || setting->program.version[0] == '\0') {
    logFatal("Program version is null or empty");
    return false;
  }
  if (setting->program.command.stop.pressMs == 0) {
    logFatal("Stop command press duration must be greater than 0 ms");
    return false;
  }
  if (setting->program.trackers.interval == 0) {
    logFatal("Trackers interval must be greater than 0");
    return false;
  }
  if (setting->program.trackers.command.debounce == 0) {
    logFatal("Trackers command debounce must be greater than 0");
    return false;
  }
  if (setting->program.ldr.sampling.interval == 0) {
    logFatal("LDR sampling interval must be greater than 0");
    return false;
  }
  if (setting->program.ldrs.threshold > 100) {
    logFatal("LDRs threshold out of range");
    return false;
  }
  if (setting->program.ldrs.night.cutoff > 100) {
    logFatal("LDRs night cutoff out of range");
    return false;
  }
  if (setting->program.motors.speed > 100 || setting->program.motors.speed == 0) {
    logFatal("Motors speed out of range");
    return false;
  }
  if (setting->program.motors.timeout == 0) {
    logFatal("Motors timeout must be greater than 0");
    return false;
  }

  return true;
}

Setting* getSetting() {
  return &setting;
}

#ifdef BOARD_ESP32
// Must be compiled only for ESP32 board, it's a memory optimization for Arduino boards
void logSetting() {
  Setting *setting = getSetting();
  Log.traceln("Setting:");
  Log.traceln("- Board:");
  Log.traceln("  - ADC resolution: %d", setting->board.adc.resolution);
  Log.traceln("  - PWM resolution: %d", setting->board.pwm.resolution);
  Log.traceln("  - Serial baud rate: %lu", setting->board.serial.baudRate);
  for (uint8_t i = 0; i < TRACKER_MAX; i++) {
    Log.traceln("  - Tracker %d:", i);
    Log.traceln("    - LDR up pin: %d", setting->board.pin.tracker[i].ldr.up);
    Log.traceln("    - LDR down pin: %d", setting->board.pin.tracker[i].ldr.down);
    Log.traceln("    - Motor ENA pin: %d", setting->board.pin.tracker[i].motors.ena);
    Log.traceln("    - Motor IN1 pin: %d", setting->board.pin.tracker[i].motors.in1);
    Log.traceln("    - Motor IN2 pin: %d", setting->board.pin.tracker[i].motors.in2);
    Log.traceln("    - Motor ENB pin: %d", setting->board.pin.tracker[i].motors.enb);
    Log.traceln("    - Motor IN3 pin: %d", setting->board.pin.tracker[i].motors.in3);
    Log.traceln("    - Motor IN4 pin: %d", setting->board.pin.tracker[i].motors.in4);
    Log.traceln("    - Tracker deploy command pin: %d", setting->board.pin.tracker[i].command.deploy);
    Log.traceln("    - Tracker retract command pin: %d", setting->board.pin.tracker[i].command.retract);
    Log.traceln("    - Tracker LED pin: %d", setting->board.pin.tracker[i].led);
  }
  Log.traceln("  - LED status pin: %d", setting->board.pin.led);
  Log.traceln("  - Stop command pin: %d", setting->board.pin.driver.command.stop);
  Log.traceln("  - Manual mode pin: %d", setting->board.pin.mode.manual);
  Log.traceln("- Program:");
  Log.traceln("  - Version: %s", setting->program.version);
  Log.traceln("  - Trackers interval: %lu", setting->program.trackers.interval);
  Log.traceln("  - Trackers command debounce: %lu", setting->program.trackers.command.debounce);
  Log.traceln("  - LDR sampling interval: %lu", setting->program.ldr.sampling.interval);
  Log.traceln("  - LDRs threshold: %d", setting->program.ldrs.threshold);
  Log.traceln("  - LDRs night cutoff: %d", setting->program.ldrs.night.cutoff);
  Log.traceln("  - Motors speed: %d", setting->program.motors.speed);
  Log.traceln("  - Motors timeout: %d", setting->program.motors.timeout);
}

#endif