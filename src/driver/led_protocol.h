#ifndef LED_PROTOCOL_H
#define LED_PROTOCOL_H

class LedProtocol {
public:
  explicit LedProtocol();
  explicit LedProtocol(uint8_t ledStatusPin);
  void init();
  void waitReady();
  void invalidSetting();
  void update();

private:
  uint8_t _ledStatusPin;
};

#endif