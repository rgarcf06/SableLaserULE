#pragma once
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

#define BLE_SERVICE_UUID        "12345678-1234-1234-1234-123456789abc"
#define BLE_CHARACTERISTIC_UUID "abcd1234-ab12-ab12-ab12-abcdef123456"

class BLEManager {
public:
  void begin(const char* nombre);
  bool hayColorNuevo();
  void getRGB(uint8_t &r, uint8_t &g, uint8_t &b);
};
