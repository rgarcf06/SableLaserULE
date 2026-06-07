#pragma once
#include <WiFi.h>
#include <WebServer.h>

class WebManager {
public:
  void begin();
  void loop();
  bool hayColorNuevo();
  void getRGB(uint8_t &r, uint8_t &g, uint8_t &b);
};
