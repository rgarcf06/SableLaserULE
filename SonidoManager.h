#pragma once
#include <HardwareSerial.h>
#include <DFMiniMp3.h>

#define SND_REPOSO      1
#define SND_ENCENDIDO   2
#define SND_MOVIMIENTO  3
#define SND_APAGADO     4
#define SND_GOLPE       5

class Mp3Notify {
public:
  typedef void* TargetType;
  static void SetTarget(TargetType) {}
  static void OnError(uint16_t errorCode) {}
  static void OnPlayFinished(DfMp3_PlaySources source, uint16_t track) {}
  static void OnPlaySourceOnline(DfMp3_PlaySources source) {}
  static void OnPlaySourceInserted(DfMp3_PlaySources source) {}
  static void OnPlaySourceRemoved(DfMp3_PlaySources source) {}
};

class SonidoManager {
public:
  SonidoManager(HardwareSerial& serial, uint8_t pinBusy);
  void begin(uint8_t volumen = 25);
  void loop();
  void reproducir(uint8_t track);
  void reproducirForzado(uint8_t track);
  bool estaReproduciendo();
  void setDuracionReposo(uint32_t ms);

private:
  DFMiniMp3<HardwareSerial, Mp3Notify> _dfPlayer;
  uint8_t _pinBusy;
  uint8_t _trackActual;
  bool    _estabaReproduciendo;
  uint32_t _inicioReproduccion;
  uint32_t _duracionReposo;
};
