#ifndef LDRS_H
#define LDRS_H
#include "ldr.h"

class Ldrs {
public:
  Ldr up;
  Ldr down;
  explicit Ldrs(Ldr up, Ldr down);
  void init();
  void update();
};

#endif