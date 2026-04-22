
#include <Arduino.h>
#include "settings.h"
#include "LDR.h"
#include "serial_print.h"

SerialPrint::SerialPrint() : eventId(1) {}

void SerialPrint::eventPrefix(const char *message) {
    Serial.print("[EVT-");
    Serial.print(eventId);
    Serial.print("] ");
    Serial.print(message);
    eventId++;
}

void SerialPrint::event(const char *message) {
    eventPrefix(message);
}

void SerialPrint::eventln(const char *message) {
    eventPrefix(message);
    Serial.println();
}

void SerialPrint::LDR(const LDR &ldr, const char *name) {
    Serial.print(name);
    Serial.print(ldr.raw);
    Serial.print(" (");
    Serial.print(ldr.percent);
    Serial.println("%)");
}

void SerialPrint::settingsPin(const SettingsPin &pin) {
    Serial.println("Pin setup completed");
    Serial.println("Settings:");
    Serial.print("  LDR day up: "); Serial.println(pin.LDR.day.up);
    Serial.print("  LDR day down: "); Serial.println(pin.LDR.day.down);
    Serial.print("  LDR night: "); Serial.println(pin.LDR.night);
    Serial.print("  Deploy button: "); Serial.println(pin.button.deploy);
    Serial.print("  Retract button: "); Serial.println(pin.button.retract);
    Serial.print("  Automatic button: "); Serial.println(pin.button.automatic);
    Serial.print("  Scan button: "); Serial.println(pin.button.scan);
}

void SerialPrint::settingsProgram(const SettingsProgram &program) {
    Serial.println("Program settings:");
    Serial.print("  Version: "); Serial.println(program.version);
    Serial.print("  LDR threshold: "); Serial.print(program.LDR.threshold); Serial.println("%");
    Serial.print("  Motor speed: "); Serial.print(program.motor.speed); Serial.println("%");
}

void SerialPrint::settings(const Settings &settings) {
    settingsPin(settings.pin);
    settingsProgram(settings.program);
}
