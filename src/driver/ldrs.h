#ifndef LDRS_H
#define LDRS_H
#include "ldr.h"

enum class ldrsComparison {
  UpGreaterThanDown,
  DownGreaterThanUp,
  Deadband,
  NotUpdated
};

class Ldrs {
public:
  Ldr up;
  Ldr down;
  explicit Ldrs(Ldr up, Ldr down);
  void init();
  ldrsComparison update();
};

#endif