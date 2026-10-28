#ifndef APP_NETWORK_SERVICE_H
#define APP_NETWORK_SERVICE_H
#include <Arduino.h>
#include "setting.h"
#include "app_status.h"

class AppNetworkService {
public:
  static void start(const SettingSecretWifi *secretWifi);
  static void publish(const AppSystemStatus &status);

private:
  static void task(void *ctx);
};

#endif
