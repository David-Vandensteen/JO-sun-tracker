#include <Arduino.h>
#include <ArduinoLog.h>
#include "setting.h"
#include "app_status.h"
#include "app_network_service.h"
#include "app_log_buffer.h"

static portMUX_TYPE sharedStatusMux = portMUX_INITIALIZER_UNLOCKED;
static AppSystemStatus sharedStatus = {"idle", {0, 0, "manual", "idle", 0}, {"released", "released", "auto", "released"}};

void AppNetworkService::publish(const AppSystemStatus &status) {
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
static char logBody[LOG_BUFFER_SIZE];

static const char STATUS_PAGE[] PROGMEM = R"HTML(<!DOCTYPE html>
<html><head><meta charset="utf-8"><title>JO Sun Tracker</title></head>
<body><h1>JO Sun Tracker</h1>
<p>Prochaine comparaison LDR dans : <strong id="countdown">--</strong> s</p>
<pre id="status">...</pre>
<h2>Logs</h2><pre id="log">...</pre>
<script>
let nextComparisonAt = null;
function updateCountdown() {
  const el = document.getElementById('countdown');
  if (nextComparisonAt === null) { el.textContent = '--'; return; }
  el.textContent = Math.max(0, Math.ceil((nextComparisonAt - Date.now()) / 1000));
}
function refresh() {
  fetch('/status').then(r => r.json()).then(d => {
    document.getElementById('status').textContent = JSON.stringify(d, null, 2);
    nextComparisonAt = Date.now() + d.status.nextComparisonMs;
    updateCountdown();
  });
}
function refreshLog() {
  fetch('/log').then(r => r.text()).then(t => {
    const el = document.getElementById('log');
    el.textContent = t;
    el.scrollTop = el.scrollHeight;
  });
}
refresh();
refreshLog();
setInterval(refresh, 1000);
setInterval(updateCountdown, 1000);
setInterval(refreshLog, 2000);
</script></body></html>)HTML";

static void handleRoot() {
  server.send(200, "text/html; charset=utf-8", STATUS_PAGE);
}

static void handleLog() {
  AppLogOutput::copyTo(logBody, sizeof(logBody));
  server.send(200, "text/plain; charset=utf-8", logBody);
}

static void handleStatus() {
  AppSystemStatus snapshot;
  portENTER_CRITICAL(&sharedStatusMux);
  snapshot = sharedStatus;
  portEXIT_CRITICAL(&sharedStatusMux);

  const Setting *setting = getSetting();
  const SettingBoardPinTracker &tracker = setting->board.pin.tracker[TRACKER_1];
  char body[1536];
  snprintf(
    body,
    sizeof(body),
    "{\"status\":{\"driver\":\"%s\",\"uptime\":%lu,\"ldrUp\":%u,\"ldrDown\":%u,\"mode\":\"%s\",\"motor\":\"%s\",\"nextComparisonMs\":%lu,"
    "\"buttons\":{\"deploy\":\"%s\",\"retract\":\"%s\",\"mode\":\"%s\",\"stop\":\"%s\"}},"
    "\"setting\":{\"version\":\"%s\","
    "\"trackers\":{\"interval\":%lu,\"commandDebounce\":%lu},"
    "\"ldr\":{\"samplingInterval\":%lu},"
    "\"ldrs\":{\"threshold\":%d,\"nightCutoff\":%d},"
    "\"motors\":{\"speed\":%d,\"timeout\":%d},"
    "\"command\":{\"stopPressMs\":%u},"
    "\"gpio\":{"
    "\"ldrUp\":%u,\"ldrDown\":%u,"
    "\"motorEna\":%u,\"motorIn1\":%u,\"motorIn2\":%u,\"motorEnb\":%u,\"motorIn3\":%u,\"motorIn4\":%u,"
    "\"trackerDeploy\":%u,\"trackerRetract\":%u,\"trackerMode\":%u,\"trackerLed\":%u,"
    "\"led\":%u,\"stop\":%u}}}",
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
    snapshot.buttons.stop,
    setting->program.version,
    (unsigned long)setting->program.trackers.interval,
    (unsigned long)setting->program.trackers.command.debounce,
    (unsigned long)setting->program.ldr.sampling.interval,
    (int)setting->program.ldrs.threshold,
    (int)setting->program.ldrs.night.cutoff,
    (int)setting->program.motors.speed,
    (int)setting->program.motors.timeout,
    (unsigned int)setting->program.command.stop.pressMs,
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
    (unsigned int)setting->board.pin.led,
    (unsigned int)setting->board.pin.driver.command.stop
  );
  server.send(200, "application/json", body);
}
#endif

void AppNetworkService::start(const SettingSecretWifi *secretWifi) {
  if (secretWifi == nullptr || strlen(secretWifi->ssid) == 0) {
    Log.warningln("Network: WiFi SSID is empty, network disabled");
    return;
  }
  wifiSetting = secretWifi;
  xTaskCreatePinnedToCore(AppNetworkService::task, "network", NETWORK_STACK_SIZE, nullptr, 1, nullptr, NETWORK_CORE);
}

void AppNetworkService::task(void *ctx) {
  (void)ctx;
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSetting->ssid, wifiSetting->password);
  unsigned long lastAttempt = millis();
  bool connected = false;
  #if FEAT_WEBSERVER
  bool serverStarted = false;
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/log", handleLog);
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
