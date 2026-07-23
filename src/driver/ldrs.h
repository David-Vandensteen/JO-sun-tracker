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
  explicit Ldrs(Ldr up, Ldr down, uint8_t threshold);
  void init();
  ldrsComparison update();

private:
  uint8_t _threshold;
};

#endif