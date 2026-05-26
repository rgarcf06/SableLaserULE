#include "SonidoManager.h"

SonidoManager::SonidoManager(HardwareSerial& serial, uint8_t pinBusy)
  : _dfPlayer(serial, nullptr), _pinBusy(pinBusy) {}

void SonidoManager::begin(uint8_t volumen) {
  pinMode(_pinBusy, INPUT);
  delay(3000);
  _dfPlayer.begin();
  delay(500);
  _dfPlayer.setVolume(volumen);
}

void SonidoManager::loop() {
  _dfPlayer.loop();
}

void SonidoManager::reproducirFondo(uint8_t track) {
  // El módulo retoma solo al terminar el advert
  _dfPlayer.playMp3FolderTrack(track);
}

void SonidoManager::reproducirAdvert(uint8_t track) {
  _dfPlayer.playAdvertisement(track);
}

void SonidoManager::reproducirForzado(uint8_t track) {
  _dfPlayer.playMp3FolderTrack(track);
}
