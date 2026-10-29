#include <Arduino.h>
#include <ArduinoLog.h>
#include <esp_system.h>
#include "setting.h"
#include "driver.h"
#include "driver_state.h"
#include "led_protocol.h"
#include "app_network_service.h"
#include "app_log_buffer.h"
#include "tracker.h"
#include "app_status.h"
#include "display.h"

static AppLogOutput logOutput;

#if FEAT_DISPLAY
static Display display;
#endif

Setting *Driver::_setting;
LedProtocol *Driver::_ledProtocol;
Tracker *Driver::_tracker;
DriverState Driver::_state;

void Driver::init(Setting *setting) {
  _setting = setting;
  AppLogOutput::init(_setting->program.app.log.bufferSize);
  _ledProtocol = new LedProtocol(_setting->board.pin.led);
  if (LOG) {
    Serial.begin(_setting->board.serial.baudRate);
    Log.begin(LOG_LEVEL, &logOutput);
    delay(LOG_BOOT_DELAY_MS);
  }
  Log.noticeln("---------");
  if (!assertSetting()) {
    Log.fatal("Invalid setting");
    _ledProtocol->fatalError();
  }
  Log.noticeln("Setting is valid");
  logSetting();
   Log.noticeln("Waiting before starting...");
  _ledProtocol->waiting();
  Log.noticeln("Initializing tracker");
  _tracker = new Tracker(
    &_setting->board.pin.tracker,
    &_setting->program.tracker,
    &_setting->program.ldr,
    &_setting->program.ldrs,
    &_setting->board.adc,
    &_setting->board.pwm,
    &_setting->program.motors
  );
  _tracker->init();
  Log.noticeln("Starting program");
  Log.noticeln("JO Sun Tracker - version %s", _setting->program.version);
  _state.setIdle();
  #if FEAT_DISPLAY
  display.init(_setting->board.pin.display, _setting->program.display);
  #endif
  AppNetworkService::start(&_setting->secret.wifi, &_setting->program);
}

void Driver::heartBeat() {
  unsigned long currentTime = millis();
  static unsigned long lastTime = 0;
  if (currentTime - lastTime >= HEARTBEAT_INTERVAL) {
    lastTime = currentTime;
    Serial.println("Heartbeat");
  }
}

void Driver::update() {
  _ledProtocol->update();
  _tracker->update();
  if (_tracker->isResetRequested()) {
    _state.setReset();
  }
  publishStatus();
  #if defined(HEARTBEAT_INTERVAL)
    if (LOG && HEARTBEAT) { heartBeat(); }
  #endif
}

static const char *buttonLevel(uint8_t pin) {
  return digitalRead(pin) == LOW ? "pressed" : "released";
}

void Driver::publishStatus() {
  AppSystemStatus status;
  status.driver = _state.isReset() ? "reset" : "idle";
  status.tracker = _tracker->AppStatus();
  const SettingBoardPinTracker &tracker = _setting->board.pin.tracker;
  status.buttons.deploy = buttonLevel(tracker.command.deploy);
  status.buttons.retract = buttonLevel(tracker.command.retract);
  status.buttons.mode = digitalRead(tracker.command.mode) == LOW ? "manual" : "auto";
  #if FEAT_DISPLAY
  display.showLdrs(status.tracker.ldrUp, status.tracker.ldrDown);
  #endif
  AppNetworkService::publish(status);
}

void Driver::reset() {
  Log.traceln("Driver::reset");
  _state.setReset();
  ESP.restart();
}

DriverState Driver::state() {
  return _state;
}
