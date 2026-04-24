#include "settings.h"
#include "logic.h"
#include "serial_print.h"

Logic::Logic(Settings *settings) : _settings(settings) {}

void Logic::initLDRs() {
  _ldrs = new LDRs(
    LDR(_settings->board.pin.ldr.day.up, _settings->board.adc.resolution),
    LDR(_settings->board.pin.ldr.day.down, _settings->board.adc.resolution),
    LDR(_settings->board.pin.ldr.night, _settings->board.adc.resolution)
  );
}

void Logic::initMotors() {
  _motors = new Motors(
    Motor(_settings->board.pin.motors.in1, _settings->board.pin.motors.in2, _settings->board.pin.motors.ena, _settings->board.pwm.resolution),
    Motor(_settings->board.pin.motors.in3, _settings->board.pin.motors.in4, _settings->board.pin.motors.enb, _settings->board.pwm.resolution)
  );
}

void Logic::init() {
  if (DEBUG) SerialPrint::eventln("Initializing logic");
  initLDRs();
  initMotors();
}

void Logic::deploy() {
  if (DEBUG) SerialPrint::eventln("Deploying motors");
  _motors->deploy(_settings->program.motor.speed);
}

void Logic::retract() {
  if (DEBUG) SerialPrint::eventln("Retracting motors");
  _motors->retract(_settings->program.motor.speed);
}

void Logic::scan() {
  if (DEBUG) SerialPrint::eventln("Scan button pressed");
  // Implement scan functionality here
}

bool Logic::getAutoMode() {
  return _isAutoMode;
}

void Logic::setAutoMode(bool autoMode) {
  _isAutoMode = autoMode;
  if (DEBUG) {
    if (_isAutoMode) {
      SerialPrint::eventln("Switched to automatic mode");
    } else {
      SerialPrint::eventln("Switched to manual mode");
      SerialPrint::eventln("Waiting for button presses...");
    }
  }
}

void Logic::runAuto() {
  unsigned long now = millis();
  unsigned long lastIterationTime = 0;

  if (now - lastIterationTime <= 1000) {
    _ldrs->read();
    bool isLDRDifferent = _ldrs->isDayUpDifferentFromDayDown(_settings->program.ldr.threshold);

    if (DEBUG && isLDRDifferent) {
      SerialPrint::event("LDR values are different with threshold ");
      Serial.print(_settings->program.ldr.threshold);
      Serial.println("%");

      SerialPrint::ldr(&_ldrs->dayUp, "  LDR day up: ");
      SerialPrint::ldr(&_ldrs->dayDown, "  LDR day down: ");
    }

    if (isLDRDifferent) {
      if (_ldrs->isDayUpBrighterThanDayDown(_settings->program.ldr.threshold)) {
        if (DEBUG) SerialPrint::eventln("Deploying motors");
        _motors->deploy(_settings->program.motor.speed);
      } else if (_ldrs->isDayDownBrighterThanDayUp(_settings->program.ldr.threshold)) {
        if (DEBUG) SerialPrint::eventln("Retracting motors");
        _motors->retract(_settings->program.motor.speed);
      }
    }
  }
}

void Logic::runManual() {
  uint8_t btn = 0;
  if (digitalRead(_settings->board.pin.button.deploy) == LOW) btn = 1;
  else if (digitalRead(_settings->board.pin.button.retract) == LOW) btn = 2;
  else if (digitalRead(_settings->board.pin.button.scan) == LOW) btn = 3;

  switch (btn) {
    case 1: deploy(); break;
    case 2: retract(); break;
    case 3: scan(); break;
    default: break;
  }
}