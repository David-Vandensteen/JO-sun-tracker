#include <Arduino.h>
#include <ArduinoLog.h>
#include "ldr.h"
#include "ldrs.h"

Ldrs::Ldrs(Ldr up, Ldr down)
  : up(up),
    down(down)
{}

void Ldrs::init() {
  Log.traceln("Ldrs::init");
  up.init();
  down.init();
}

void Ldrs::update() {
  unsigned long now = millis();
  up.update(now);
  down.update(now);
}
