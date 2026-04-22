#ifndef SERIAL_PRINT_H
#define SERIAL_PRINT_H
#include "settings.h"
#include "LDR.h"

class SerialPrint {
public:
	SerialPrint();
	void event(const char *message);
	void eventln(const char *message);
	void LDR(const LDR &ldr, const char *name);
	void settingsPin(const SettingsPin &pin);
	void settingsProgram(const SettingsProgram &program);
	void settings(const Settings &settings);
private:
	uint16_t eventId;
	void eventPrefix(const char *message);
};

#endif