#ifndef LDR_H
#define LDR_H
#include <Arduino.h>

typedef struct LDR {
  int raw;
  int percent;
} LDR;

typedef struct LDRs {
  LDR dayUp;
  LDR dayDown;
  LDR night;
} LDRs;

void LDRReadByPin(LDR* ldr, uint8_t pin, int analogResolution);
void LDRsRead(LDRs* ldrs, int analogResolution);

#endif