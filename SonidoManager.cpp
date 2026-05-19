#include "SonidoManager.h"

SonidoManager::SonidoManager(HardwareSerial& serial, uint8_t pinBusy)
  : _dfPlayer(serial, nullptr), _pinBusy(pinBusy), _trackActual(0),
    _estabaReproduciendo(false), _inicioReproduccion(0), _duracionReposo(0) {}

void SonidoManager::loop() {
  _dfPlayer.loop();
}

void SonidoManager::begin(uint8_t volumen) {
  pinMode(_pinBusy, INPUT);
  delay(3000);
  _dfPlayer.begin();
  delay(500);
  _dfPlayer.setVolume(volumen);
}

void SonidoManager::setDuracionReposo(uint32_t ms) {
  _duracionReposo = ms;
}

void SonidoManager::reproducir(uint8_t track) {
  bool reproduciendo = estaReproduciendo();
  uint32_t ahora = millis();

  if (_trackActual == track) {
    // Relanzar por timer (50ms antes de que acabe) o por PIN_BUSY
    bool porTimer = (_duracionReposo > 0 && 
                     (ahora - _inicioReproduccion) >= (_duracionReposo - 50));
    bool porPin   = (_estabaReproduciendo && !reproduciendo);

    if (porTimer || porPin) {
      _inicioReproduccion = ahora;
      _dfPlayer.playMp3FolderTrack(track);
    }
  } else if (!reproduciendo) {
    _trackActual = track;
    _inicioReproduccion = ahora;
    _dfPlayer.playMp3FolderTrack(track);
  }

  _estabaReproduciendo = reproduciendo;
}

void SonidoManager::reproducirForzado(uint8_t track) {
  _trackActual = track;
  _dfPlayer.playMp3FolderTrack(track);
}

bool SonidoManager::estaReproduciendo() {
  return (digitalRead(_pinBusy) == LOW);
}
