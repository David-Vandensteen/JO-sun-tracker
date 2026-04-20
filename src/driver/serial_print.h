#ifndef SERIAL_PRINT_H
#define SERIAL_PRINT_H

void serialPrintEvent(const char *message, uint16_t &eventId);
void serialPrintlnEvent(const char *message, uint16_t &eventId);
void serialPrintLDR(LDR ldr, const char *name);
void serialPrintSettingsPin(SettingsPin pin);
void serialPrintSettingsProgram(SettingsProgram program);
void serialPrintSettings(Settings settings);

#endif