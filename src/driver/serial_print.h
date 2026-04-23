#ifndef SERIAL_PRINT_H
#define SERIAL_PRINT_H
#include "settings.h"
#include "ldr.h"

class SerialPrint {
public:
	SerialPrint();
	void event(const char *message);
	void eventln(const char *message);
	static void settingsBoardPin(const SettingsBoardPin *pin);
	static void settingsProgram(const SettingsProgram *program);
	static void globalSettings(const Settings *settings);
	static void ldr(const LDR *ldr, const char *name);
private:
	uint16_t eventId;
	void eventPrefix(const char *message);
};

#endif