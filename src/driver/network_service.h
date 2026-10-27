#ifndef NETWORK_SERVICE_H
#define NETWORK_SERVICE_H
#include <Arduino.h>
#include "setting.h"
#include "status.h"

class NetworkService {
public:
  static void start(const SettingSecretWifi *secretWifi);
  static void publish(const SystemStatus &status);

private:
  static void task(void *ctx);
};

#endif
