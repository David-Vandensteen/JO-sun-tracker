#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "status.h"
#include "network_service.h"

static portMUX_TYPE sharedStatusMux = portMUX_INITIALIZER_UNLOCKED;
static SystemStatus sharedStatus = {"idle", {0, 0, "manual", "idle"}};

void NetworkService::publish(const SystemStatus &status) {
  portENTER_CRITICAL(&sharedStatusMux);
  sharedStatus = status;
  portEXIT_CRITICAL(&sharedStatusMux);
}

#if FEAT_WIFI
#include <WiFi.h>

static const SettingSecretWifi *wifiSetting = nullptr;
static const BaseType_t NETWORK_CORE = 0;
static const uint32_t NETWORK_STACK_SIZE = 8192;
static const unsigned long WIFI_RECONNECT_INTERVAL = 10000;

#if FEAT_WEBSERVER
#include <WebServer.h>

static WebServer server(80);

static const char STATUS_PAGE[] PROGMEM = R"HTML(<!DOCTYPE html>
<html><head><meta charset="utf-8"><title>JO Sun Tracker</title></head>
<body><h1>JO Sun Tracker</h1><pre id="status">...</pre>
<script>
function refresh() {
  fetch('/status').then(r => r.json()).then(d => {
    document.getElementById('status').textContent = JSON.stringify(d, null, 2);
  });
}
refresh();
setInterval(refresh, 1000);
</script></body></html>)HTML";

static void handleRoot() {
  server.send(200, "text/html; charset=utf-8", STATUS_PAGE);
}

static void handleStatus() {
  SystemStatus snapshot;
  portENTER_CRITICAL(&sharedStatusMux);
  snapshot = sharedStatus;
  portEXIT_CRITICAL(&sharedStatusMux);

  char body[256];
  snprintf(
    body,
    sizeof(body),
    "{\"driver\":\"%s\",\"uptime\":%lu,\"ldrUp\":%u,\"ldrDown\":%u,\"mode\":\"%s\",\"motor\":\"%s\"}",
    snapshot.driver,
    millis() / 1000,
    snapshot.tracker.ldrUp,
    snapshot.tracker.ldrDown,
    snapshot.tracker.mode,
    snapshot.tracker.motor
  );
  server.send(200, "application/json", body);
}
#endif

void NetworkService::start(const SettingSecretWifi *secretWifi) {
  if (secretWifi == nullptr || strlen(secretWifi->ssid) == 0) {
    Log.warningln("Network: WiFi SSID is empty, network disabled");
    return;
  }
  wifiSetting = secretWifi;
  xTaskCreatePinnedToCore(NetworkService::task, "network", NETWORK_STACK_SIZE, nullptr, 1, nullptr, NETWORK_CORE);
}

void NetworkService::task(void *ctx) {
  (void)ctx;
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSetting->ssid, wifiSetting->password);
  unsigned long lastAttempt = millis();
  bool connected = false;
  #if FEAT_WEBSERVER
  bool serverStarted = false;
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  #endif

  for (;;) {
    if (WiFi.status() == WL_CONNECTED) {
      if (!connected) {
        connected = true;
        Log.noticeln("Network: connected, IP %s", WiFi.localIP().toString().c_str());
      }
      #if FEAT_WEBSERVER
      if (!serverStarted) {
        server.begin();
        serverStarted = true;
        Log.noticeln("Network: web server started on port 80");
      }
      server.handleClient();
      #endif
    } else {
      connected = false;
      if (millis() - lastAttempt >= WIFI_RECONNECT_INTERVAL) {
        lastAttempt = millis();
        Log.warningln("Network: WiFi not connected, reconnecting");
        WiFi.reconnect();
      }
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}
#endif
