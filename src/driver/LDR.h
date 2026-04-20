#ifndef LDR_H
#define LDR_H
#include <Arduino.h>

class LDR {
public:
    int raw;
    int percent;
    LDR();
    void read(uint8_t pin, int analogResolution);
};

class LDRs {
public:
    LDR dayUp;
    LDR dayDown;
    LDR night;
    void read(uint8_t pinUp, uint8_t pinDown, uint8_t pinNight, int analogResolution);
};

#endif