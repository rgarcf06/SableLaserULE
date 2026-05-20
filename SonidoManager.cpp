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
  uint32_t ahora = millis();
  bool reproduciendo = estaReproduciendo();

  if (_trackActual != track) {
    // Interrumpir inmediatamente para nuevo sonido (ej: de reposo a movimiento)
    _trackActual = track;
    _inicioReproduccion = ahora;
    _dfPlayer.playMp3FolderTrack(track);
  } else {
    // Es el mismo track: solo relanzar si ha terminado
    bool porTimer = (_duracionReposo > 0 && 
                     (ahora - _inicioReproduccion) >= (_duracionReposo - 50));
    bool porPin   = (_estabaReproduciendo && !reproduciendo);

    if (porTimer || porPin) {
      _inicioReproduccion = ahora;
      _dfPlayer.playMp3FolderTrack(track);
    }
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
