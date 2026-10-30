#include <ArduinoLog.h>
#include "setting.h"

static Setting setting;

void logFatal(const char* message) {
  #if LOG_FATAL_SETTING
  Log.fatal(message);
  #endif
}

// Returns false when the pin is already registered by another setting or when the table is full
static bool registerPin(uint8_t pin, const char *name, uint8_t *pins, const char **names, uint8_t &count, uint8_t capacity) {
  for (uint8_t i = 0; i < count; i++) {
    if (pins[i] == pin) {
      #if LOG_FATAL_SETTING
      Log.fatal("Pin %d used by both '%s' and '%s'", pin, names[i], name);
      #endif
      return false;
    }
  }
  if (count >= capacity) {
    logFatal("Too many pins registered");
    return false;
  }
  pins[count] = pin;
  names[count] = name;
  count++;
  return true;
}

static bool assertPinsUnique(Setting *setting) {
  constexpr uint8_t PIN_CAPACITY = 16;
  uint8_t pins[PIN_CAPACITY];
  const char *names[PIN_CAPACITY];
  uint8_t count = 0;
  SettingBoardPinTracker &pin = setting->board.pin.tracker;
  if (!registerPin(pin.ldr.up, "LDR up", pins, names, count, PIN_CAPACITY)) return false;
  if (!registerPin(pin.ldr.down, "LDR down", pins, names, count, PIN_CAPACITY)) return false;
  if (!registerPin(pin.motors.ena, "Motor ENA", pins, names, count, PIN_CAPACITY)) return false;
  if (!registerPin(pin.motors.in1, "Motor IN1", pins, names, count, PIN_CAPACITY)) return false;
  if (!registerPin(pin.motors.in2, "Motor IN2", pins, names, count, PIN_CAPACITY)) return false;
  if (!registerPin(pin.motors.enb, "Motor ENB", pins, names, count, PIN_CAPACITY)) return false;
  if (!registerPin(pin.motors.in3, "Motor IN3", pins, names, count, PIN_CAPACITY)) return false;
  if (!registerPin(pin.motors.in4, "Motor IN4", pins, names, count, PIN_CAPACITY)) return false;
  if (!registerPin(pin.command.deploy, "Tracker deploy command", pins, names, count, PIN_CAPACITY)) return false;
  if (!registerPin(pin.command.retract, "Tracker retract command", pins, names, count, PIN_CAPACITY)) return false;
  if (!registerPin(pin.command.mode, "Tracker mode command", pins, names, count, PIN_CAPACITY)) return false;
  if (!registerPin(pin.led, "Tracker LED", pins, names, count, PIN_CAPACITY)) return false;
  if (!registerPin(setting->board.pin.led, "LED status", pins, names, count, PIN_CAPACITY)) return false;
  if (!registerPin(setting->board.pin.display.sda, "Display SDA", pins, names, count, PIN_CAPACITY)) return false;
  if (!registerPin(setting->board.pin.display.scl, "Display SCL", pins, names, count, PIN_CAPACITY)) return false;
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
  if (setting->board.pin.tracker.ldr.up == 0) {
    logFatal("LDR up is invalid");
    return false;
  }
  if (setting->board.pin.tracker.ldr.down == 0) {
    logFatal("LDR down is invalid");
    return false;
  }
  if (setting->board.pin.tracker.motors.ena == 0) {
    logFatal("Motor ENA pin is invalid");
    return false;
  }
  if (setting->board.pin.tracker.motors.in1 == 0) {
    logFatal("Motor IN1 pin is invalid");
    return false;
  }
  if (setting->board.pin.tracker.motors.in2 == 0) {
    logFatal("Motor IN2 pin is invalid");
    return false;
  }
  if (setting->board.pin.tracker.motors.enb == 0) {
    logFatal("Motor ENB pin is invalid");
    return false;
  }
  if (setting->board.pin.tracker.motors.in3 == 0) {
    logFatal("Motor IN3 pin is invalid");
    return false;
  }
  if (setting->board.pin.tracker.motors.in4 == 0) {
    logFatal("Motor IN4 pin is invalid");
    return false;
  }
  if (setting->board.pin.tracker.command.deploy == 0) {
    logFatal("Tracker deploy command pin is invalid");
    return false;
  }
  if (setting->board.pin.tracker.command.retract == 0) {
    logFatal("Tracker retract command pin is invalid");
    return false;
  }
  if (setting->board.pin.tracker.command.mode == 0) {
    logFatal("Tracker mode command pin is invalid");
    return false;
  }
  if (setting->board.pin.tracker.led == 0) {
    logFatal("Tracker LED pin is invalid");
    return false;
  }
  if (setting->board.pin.led == 0) {
    logFatal("LED pin is invalid");
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
  if (setting->program.tracker.interval == 0) {
    logFatal("Tracker interval must be greater than 0");
    return false;
  }
  if (setting->program.tracker.command.debounce == 0) {
    logFatal("Tracker command debounce must be greater than 0");
    return false;
  }
  if (setting->program.tracker.command.resetHoldMs == 0) {
    logFatal("Tracker command reset hold must be greater than 0");
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
  if (setting->secret.wifi.ssid == nullptr) {
    logFatal("WiFi SSID is null");
    return false;
  }
  if (setting->secret.wifi.password == nullptr) {
    logFatal("WiFi password is null");
    return false;
  }

  return true;
}

Setting* getSetting() {
  return &setting;
}

void logSetting() {
  Setting *setting = getSetting();
  Log.traceln("Setting:");
  Log.traceln("- Board:");
  Log.traceln("  - ADC resolution: %d", setting->board.adc.resolution);
  Log.traceln("  - PWM resolution: %d", setting->board.pwm.resolution);
  Log.traceln("  - Serial baud rate: %lu", setting->board.serial.baudRate);
  Log.traceln("- Tracker:");
  Log.traceln("  - LDR up pin: %d", setting->board.pin.tracker.ldr.up);
  Log.traceln("  - LDR down pin: %d", setting->board.pin.tracker.ldr.down);
  Log.traceln("  - Motor ENA pin: %d", setting->board.pin.tracker.motors.ena);
  Log.traceln("  - Motor IN1 pin: %d", setting->board.pin.tracker.motors.in1);
  Log.traceln("  - Motor IN2 pin: %d", setting->board.pin.tracker.motors.in2);
  Log.traceln("  - Motor ENB pin: %d", setting->board.pin.tracker.motors.enb);
  Log.traceln("  - Motor IN3 pin: %d", setting->board.pin.tracker.motors.in3);
  Log.traceln("  - Motor IN4 pin: %d", setting->board.pin.tracker.motors.in4);
  Log.traceln("  - Tracker deploy command pin: %d", setting->board.pin.tracker.command.deploy);
  Log.traceln("  - Tracker retract command pin: %d", setting->board.pin.tracker.command.retract);
  Log.traceln("  - Tracker mode command pin: %d", setting->board.pin.tracker.command.mode);
  Log.traceln("  - Tracker LED pin: %d", setting->board.pin.tracker.led);
  Log.traceln("  - LED status pin: %d", setting->board.pin.led);
  Log.traceln("- Program:");
  Log.traceln("  - Version: %s", setting->program.version);
  Log.traceln("  - Tracker interval: %lu", setting->program.tracker.interval);
  Log.traceln("  - Tracker invert direction: %s", setting->program.tracker.invertDirection ? "true" : "false");
  Log.traceln("  - Tracker command debounce: %lu", setting->program.tracker.command.debounce);
  Log.traceln("  - Tracker command reset hold: %lu", setting->program.tracker.command.resetHoldMs);
  Log.traceln("  - LDR sampling interval: %lu", setting->program.ldr.sampling.interval);
  Log.traceln("  - LDRs threshold: %d", setting->program.ldrs.threshold);
  Log.traceln("  - LDRs night cutoff: %d", setting->program.ldrs.night.cutoff);
  Log.traceln("  - Motors speed: %d", setting->program.motors.speed);
  Log.traceln("  - Motors timeout: %lu", setting->program.motors.timeout);
  Log.traceln("- Secret:");
  Log.traceln("  - WiFi SSID: %s", setting->secret.wifi.ssid[0] == '\0' ? "empty" : "set");
  Log.traceln("  - WiFi password: %s", setting->secret.wifi.password[0] == '\0' ? "empty" : "set");
}
