#include <Arduino.h>
#include "setting.h"
#include "setting_secret_wifi.h"

#define SETTING_BOARD_ADC_RESOLUTION 4095
#define SETTING_BOARD_PWM_RESOLUTION 255
#define SETTING_BOARD_SERIAL_BAUD_RATE 115200
#define SETTING_PROGRAM_TRACKER_INTERVAL 3 * (60 * 1000)
#define SETTING_PROGRAM_TRACKER_INVERT_DIRECTION false
#define SETTING_PROGRAM_LDR_SAMPLING_INTERVAL 1000
#define SETTING_PROGRAM_LDRS_THRESHOLD 10
#define SETTING_PROGRAM_LDRS_NIGHT_CUTOFF 10
#define SETTING_PROGRAM_MOTORS_SPEED 30
#define SETTING_PROGRAM_MOTORS_TIMEOUT 1000
#define SETTING_PROGRAM_TRACKER_COMMAND_DEBOUNCE 10
#define SETTING_PROGRAM_TRACKER_COMMAND_RESET_HOLD_MS 5000

// GPIO assignment: avoid boot strapping pins (0, 2, 5, 12, 15), flash pins (6-11), PSRAM pins (16, 17 on WROVER) and ADC2 pins for inputs read while WiFi is active
// Buttons use internal pull-ups, LDRs use ADC1 input-only pins
#define SETTING_PIN_TRACKER_COMMAND_DEPLOY 18
#define SETTING_PIN_TRACKER_COMMAND_RETRACT 19
#define SETTING_PIN_TRACKER_COMMAND_MODE 33
#define SETTING_PIN_DISPLAY_SDA 21
#define SETTING_PIN_DISPLAY_SCL 23

#define SETTING_PROGRAM_DISPLAY_ADDRESS 0x3C
#define SETTING_PROGRAM_DISPLAY_WIDTH 128
#define SETTING_PROGRAM_DISPLAY_HEIGHT 64

#define SETTING_PROGRAM_WEBSERVER_PORT 80

#define SETTING_PROGRAM_LOG_BUFFER_SIZE 4096

#define SETTING_PROGRAM_NETWORK_TASK_STACK_SIZE 8192
#define SETTING_PROGRAM_NETWORK_WIFI_RECONNECT_INTERVAL 10000

Setting* settingInit() {
  Setting *setting = getSetting();
  setting->board.adc.resolution = SETTING_BOARD_ADC_RESOLUTION;
  setting->board.pwm.resolution = SETTING_BOARD_PWM_RESOLUTION;
  setting->board.serial.baudRate = SETTING_BOARD_SERIAL_BAUD_RATE;
  setting->board.pin.tracker.ldr.up = 34;
  setting->board.pin.tracker.ldr.down = 35;
  setting->board.pin.tracker.motors.ena = 25;
  setting->board.pin.tracker.motors.in1 = 26;
  setting->board.pin.tracker.motors.in2 = 27;
  setting->board.pin.tracker.motors.enb = 14;
  setting->board.pin.tracker.motors.in3 = 22;
  setting->board.pin.tracker.motors.in4 = 13;
  setting->board.pin.tracker.command.deploy = SETTING_PIN_TRACKER_COMMAND_DEPLOY;
  setting->board.pin.tracker.command.retract = SETTING_PIN_TRACKER_COMMAND_RETRACT;
  setting->board.pin.tracker.command.mode = SETTING_PIN_TRACKER_COMMAND_MODE;
  setting->board.pin.tracker.led = 4;
  setting->board.pin.display.sda = SETTING_PIN_DISPLAY_SDA;
  setting->board.pin.display.scl = SETTING_PIN_DISPLAY_SCL;
  setting->program.display.address = SETTING_PROGRAM_DISPLAY_ADDRESS;
  setting->program.display.width = SETTING_PROGRAM_DISPLAY_WIDTH;
  setting->program.display.height = SETTING_PROGRAM_DISPLAY_HEIGHT;
  setting->program.app.webServer.port = SETTING_PROGRAM_WEBSERVER_PORT;
  setting->program.app.log.bufferSize = SETTING_PROGRAM_LOG_BUFFER_SIZE;
  setting->program.app.network.taskStackSize = SETTING_PROGRAM_NETWORK_TASK_STACK_SIZE;
  setting->program.app.network.wifiReconnectInterval = SETTING_PROGRAM_NETWORK_WIFI_RECONNECT_INTERVAL;
  setting->program.version = VERSION;
  setting->program.tracker.interval = SETTING_PROGRAM_TRACKER_INTERVAL;
  setting->program.tracker.invertDirection = SETTING_PROGRAM_TRACKER_INVERT_DIRECTION;
  setting->program.tracker.command.debounce = SETTING_PROGRAM_TRACKER_COMMAND_DEBOUNCE;
  setting->program.tracker.command.resetHoldMs = SETTING_PROGRAM_TRACKER_COMMAND_RESET_HOLD_MS;
  setting->program.ldr.sampling.interval = SETTING_PROGRAM_LDR_SAMPLING_INTERVAL;
  setting->program.ldrs.threshold = SETTING_PROGRAM_LDRS_THRESHOLD;
  setting->program.ldrs.night.cutoff = SETTING_PROGRAM_LDRS_NIGHT_CUTOFF;
  setting->program.motors.speed = SETTING_PROGRAM_MOTORS_SPEED;
  setting->program.motors.timeout = SETTING_PROGRAM_MOTORS_TIMEOUT;
  setting->secret.wifi.ssid = SETTING_SECRET_WIFI_SSID;
  setting->secret.wifi.password = SETTING_SECRET_WIFI_PASSWORD;
  return setting;
}
