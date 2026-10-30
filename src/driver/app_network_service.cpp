#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "app_status.h"
#include "app_network_service.h"
#include "app_log_buffer.h"
#include "app_status_page.h"

static portMUX_TYPE sharedStatusMux = portMUX_INITIALIZER_UNLOCKED;
static AppSystemStatus sharedStatus = {"idle", {0, 0, "manual", "idle", 0}, {"released", "released", "auto"}};

void AppNetworkService::publish(const AppSystemStatus &status) {
  portENTER_CRITICAL(&sharedStatusMux);
  sharedStatus = status;
  portEXIT_CRITICAL(&sharedStatusMux);
}

#if FEAT_WIFI
#include <WiFi.h>

static const SettingSecretWifi *wifiSetting = nullptr;
static const BaseType_t NETWORK_CORE = 0;
static uint32_t networkStackSize = 8192;
static unsigned long wifiReconnectInterval = 10000;

#if FEAT_WEBSERVER
#include <WebServer.h>

static WebServer *server = nullptr;
static uint16_t webServerPort = 80;
static char *logBody = nullptr;
static size_t logBodySize = 0;

static void handleRoot() {
  server->send(200, "text/html; charset=utf-8", STATUS_PAGE);
}

static void handleLog() {
  if (logBody == nullptr) {
    server->send(500, "text/plain; charset=utf-8", "log buffer unavailable");
    return;
  }
  AppLogOutput::copyTo(logBody, logBodySize);
  server->send(200, "text/plain; charset=utf-8", logBody);
}

static void handleStatus() {
  AppSystemStatus snapshot;
  portENTER_CRITICAL(&sharedStatusMux);
  snapshot = sharedStatus;
  portEXIT_CRITICAL(&sharedStatusMux);

  const Setting *setting = getSetting();
  const SettingBoardPinTracker &tracker = setting->board.pin.tracker;
  char body[1536];
  int written = snprintf(
    body,
    sizeof(body),
    "{\"status\":{\"driver\":\"%s\",\"uptime\":%lu,\"ldrUp\":%u,\"ldrDown\":%u,\"mode\":\"%s\",\"motor\":\"%s\",\"nextComparisonMs\":%lu,"
    "\"buttons\":{\"deploy\":\"%s\",\"retract\":\"%s\",\"mode\":\"%s\"}},"
    "\"setting\":{\"version\":\"%s\","
    "\"tracker\":{\"interval\":%lu,\"commandDebounce\":%lu,\"commandResetHoldMs\":%lu},"
    "\"ldr\":{\"samplingInterval\":%lu},"
    "\"ldrs\":{\"threshold\":%d,\"nightCutoff\":%d},"
    "\"motors\":{\"speed\":%d,\"timeout\":%lu},"
    "\"gpio\":{"
    "\"ldrUp\":%u,\"ldrDown\":%u,"
    "\"motorEna\":%u,\"motorIn1\":%u,\"motorIn2\":%u,\"motorEnb\":%u,\"motorIn3\":%u,\"motorIn4\":%u,"
    "\"trackerDeploy\":%u,\"trackerRetract\":%u,\"trackerMode\":%u,\"trackerLed\":%u,"
    "\"led\":%u}}}",
    snapshot.driver,
    millis() / 1000,
    snapshot.tracker.ldrUp,
    snapshot.tracker.ldrDown,
    snapshot.tracker.mode,
    snapshot.tracker.motor,
    snapshot.tracker.nextComparisonMs,
    snapshot.buttons.deploy,
    snapshot.buttons.retract,
    snapshot.buttons.mode,
    setting->program.version,
    (unsigned long)setting->program.tracker.interval,
    (unsigned long)setting->program.tracker.command.debounce,
    (unsigned long)setting->program.tracker.command.resetHoldMs,
    (unsigned long)setting->program.ldr.sampling.interval,
    (int)setting->program.ldrs.threshold,
    (int)setting->program.ldrs.night.cutoff,
    (int)setting->program.motors.speed,
    (unsigned long)setting->program.motors.timeout,
    (unsigned int)tracker.ldr.up,
    (unsigned int)tracker.ldr.down,
    (unsigned int)tracker.motors.ena,
    (unsigned int)tracker.motors.in1,
    (unsigned int)tracker.motors.in2,
    (unsigned int)tracker.motors.enb,
    (unsigned int)tracker.motors.in3,
    (unsigned int)tracker.motors.in4,
    (unsigned int)tracker.command.deploy,
    (unsigned int)tracker.command.retract,
    (unsigned int)tracker.command.mode,
    (unsigned int)tracker.led,
    (unsigned int)setting->board.pin.led
  );
  if (written < 0 || (size_t)written >= sizeof(body)) {
    Log.warningln("Network: /status buffer too small (%d bytes needed)", written);
    server->send(500, "text/plain; charset=utf-8", "status buffer too small");
    return;
  }
  server->send(200, "application/json", body);
}
#endif

void AppNetworkService::start(const SettingSecretWifi *secretWifi, const SettingProgram *program) {
  if (secretWifi == nullptr || strlen(secretWifi->ssid) == 0) {
    Log.warningln("Network: WiFi SSID is empty, network disabled");
    return;
  }
  wifiSetting = secretWifi;
  networkStackSize = program->app.network.taskStackSize;
  wifiReconnectInterval = program->app.network.wifiReconnectInterval;
  #if FEAT_WEBSERVER
  webServerPort = program->app.webServer.port;
  logBodySize = program->app.log.bufferSize;
  logBody = (char *)malloc(logBodySize);
  if (logBody == nullptr) {
    logBodySize = 0;
    Log.warningln("Network: log buffer allocation failed, /log disabled");
  }
  #endif
  xTaskCreatePinnedToCore(AppNetworkService::task, "network", networkStackSize, nullptr, 1, nullptr, NETWORK_CORE);
}

void AppNetworkService::task(void *ctx) {
  (void)ctx;
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSetting->ssid, wifiSetting->password);
  unsigned long lastAttempt = millis();
  bool connected = false;
  #if FEAT_WEBSERVER
  bool serverStarted = false;
  server = new WebServer(webServerPort);
  server->on("/", handleRoot);
  server->on("/status", handleStatus);
  server->on("/log", handleLog);
  #endif

  for (;;) {
    if (WiFi.status() == WL_CONNECTED) {
      if (!connected) {
        connected = true;
        Log.noticeln("Network: connected, IP %s", WiFi.localIP().toString().c_str());
      }
      #if FEAT_WEBSERVER
      if (!serverStarted) {
        server->begin();
        serverStarted = true;
        Log.noticeln("Network: web server started on port %d", webServerPort);
      }
      server->handleClient();
      #endif
    } else {
      connected = false;
      if (millis() - lastAttempt >= wifiReconnectInterval) {
        lastAttempt = millis();
        Log.warningln("Network: WiFi not connected, reconnecting");
        WiFi.reconnect();
      }
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}
#endif
