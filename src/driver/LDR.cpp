#include "LDR.h"
#include "settings.h"

void LDRReadByPin(LDR* ldr, uint8_t pin, int analogResolution) {
  ldr->raw = analogRead(pin);
  ldr->percent = map(ldr->raw, 0, analogResolution, 100, 0);
}

void LDRsRead(LDRs* ldrs, int analogResolution) {
  LDRReadByPin(&ldrs->dayUp, settings.pin.LDR.day.up, analogResolution);
  LDRReadByPin(&ldrs->dayDown, settings.pin.LDR.day.down, analogResolution);
  LDRReadByPin(&ldrs->night, settings.pin.LDR.night, analogResolution);
}
