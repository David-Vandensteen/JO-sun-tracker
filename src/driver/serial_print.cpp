
#include <Arduino.h>
#include "ldr.h"
#include "settings.h"
#include "serial_print.h"

uint16_t SerialPrint::eventId = 1;

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

void SerialPrint::ldr(const LDR *ldr, const char *name) {
    Serial.print(name);
    Serial.print(ldr->raw);
    Serial.print(" (");
    Serial.print(ldr->percent);
    Serial.println("%)");
}

void SerialPrint::settingsBoardPin(const SettingsBoardPin *pin) {
    Serial.println("Pin setup completed");
    Serial.println("Settings:");
    Serial.print("  LDR day up: "); Serial.println(pin->ldr.day.up);
    Serial.print("  LDR day down: "); Serial.println(pin->ldr.day.down);
    Serial.print("  LDR night: "); Serial.println(pin->ldr.night);
    Serial.print("  Deploy button: "); Serial.println(pin->button.deploy);
    Serial.print("  Retract button: "); Serial.println(pin->button.retract);
    Serial.print("  Automatic button: "); Serial.println(pin->button.automatic);
    Serial.print("  Scan button: "); Serial.println(pin->button.scan);
}

void SerialPrint::settingsProgram(const SettingsProgram *program) {
    Serial.println("Program settings:");
    Serial.print("  Version: "); Serial.println(program->version);
    Serial.print("  LDR threshold: "); Serial.print(program->ldr.threshold); Serial.println("%");
    Serial.print("  Motor speed: "); Serial.print(program->motor.speed); Serial.println("%");
}

void SerialPrint::globalSettings(const Settings *settings) {
    SerialPrint::settingsBoardPin(&settings->board.pin);
    SerialPrint::settingsProgram(&settings->program);
}
