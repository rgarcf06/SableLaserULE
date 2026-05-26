#include "BLEManager.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

static bool    _colorNuevo = false;
static uint8_t _r = 0, _g = 0, _b = 255;

class MiCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* c) override {
    String val = c->getValue().c_str();
    int c1 = val.indexOf(',');
    int c2 = val.indexOf(',', c1 + 1);
    if (c1 > 0 && c2 > 0) {
      _r = val.substring(0, c1).toInt();
      _g = val.substring(c1 + 1, c2).toInt();
      _b = val.substring(c2 + 1).toInt();
      _colorNuevo = true;
    }
  }
};

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) override {
    Serial.println("Cliente conectado");
  }
  void onDisconnect(BLEServer* pServer) override {
    Serial.println("Cliente desconectado");
    delay(200);
    BLEDevice::startAdvertising();
  }
};

static BLECharacteristic* _caracteristica;

void BLEManager::begin(const char* nombre) {
  BLEDevice::deinit(true);
  delay(200);
  BLEDevice::init(nombre);
  BLEServer* server = BLEDevice::createServer();
  server->setCallbacks(new ServerCallbacks());

  BLEService* service = server->createService(BLE_SERVICE_UUID);
  _caracteristica = service->createCharacteristic(
    BLE_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );
  _caracteristica->addDescriptor(new BLE2902());
  _caracteristica->setCallbacks(new MiCallback());

  service->start();
  BLEAdvertising* adv = BLEDevice::getAdvertising();
  adv->addServiceUUID(BLE_SERVICE_UUID);
  adv->setScanResponse(true);
  adv->start();
}

bool BLEManager::hayColorNuevo() {
  if (_colorNuevo) { _colorNuevo = false; return true; }
  return false;
}

void BLEManager::getRGB(uint8_t &r, uint8_t &g, uint8_t &b) {
  r = _r; g = _g; b = _b;
}
