#include "SableLED.h"

// ─── Constructor ────────────────────────────────────────────────────────────
SableLED::SableLED(uint8_t pin, uint16_t numLeds, uint8_t velocidad)
  : _strip(numLeds, pin, NEO_GRB + NEO_KHZ800),
    _color(Adafruit_NeoPixel::Color(0, 0, 255)),
    _velocidad(velocidad),
    _velocidadApagado(velocidad),
    _encendido(false),
    _animando(false),
    _encendiendose(false),
    _pixelActual(0),
    _ultimoPaso(0),
    _enGolpe(false),
    _tiempoGolpe(0) {}

// ─── Inicialización ─────────────────────────────────────────────────────────
void SableLED::begin() {
  _strip.begin();
  _strip.setBrightness(150);
  _strip.clear();
  _strip.show();
  delay(100);
}

// ─── Encender / apagar ──────────────────────────────────────────────────────
void SableLED::toggleEncendido() {
  _encendido     = !_encendido;
  _encendiendose = _encendido;
  _animando      = true;
  _pixelActual   = 0;
  _ultimoPaso    = millis();
}

bool SableLED::estaEncendido() {
  return _encendido;
}

// ─── Color ──────────────────────────────────────────────────────────────────
void SableLED::setColor(uint32_t color) {
  _color = color;
  if (_encendido && !_animando) {
    _strip.fill(_color);
    _strip.show();
  }
}

// ─── Golpe (no bloqueante) ──────────────────────────────────────────────────
void SableLED::golpe() {
  if (!_encendido) return;

  _enGolpe    = true;
  _animando   = true;
  _tiempoGolpe = millis();

  _strip.fill(Adafruit_NeoPixel::Color(255, 255, 255));
  _strip.setBrightness(200);
  _strip.show();
}

// ─── Update (llamar en cada loop) ───────────────────────────────────────────
void SableLED::update() {

  // — Resolucion del golpe —
  if (_enGolpe) {
    if ((millis() - _tiempoGolpe) > 140) {
      _enGolpe  = false;
      _animando = false;
      _strip.fill(_color);
      _strip.setBrightness(150);
      _strip.show();
    }
    return; // mientras dura el golpe no hacemos más
  }

  // — Animación encendido / apagado —
  if (!_animando) return;

  uint32_t ahora = millis();
  if ((ahora - _ultimoPaso) < _velocidad) return;
  _ultimoPaso = ahora;

  int mitad = _strip.numPixels() / 2;
  int paso  = 4; // LEDs por step

  if (_encendiendose) {
    // Encendido: desde el centro hacia los extremos
    for (int i = 0; i < paso && _pixelActual <= mitad; i++) {
      _strip.setPixelColor(_pixelActual, _color);
      _strip.setPixelColor(_strip.numPixels() - 1 - _pixelActual, _color);
      _pixelActual++;
    }
    _strip.show();

    if (_pixelActual > mitad) {
      _animando = false;
    }

  } else {
    // Apagado: desde los extremos hacia el centro
    for (int i = 0; i < paso && _pixelActual <= mitad; i++) {
      _strip.setPixelColor(mitad - _pixelActual, 0);
      _strip.setPixelColor(_strip.numPixels() - 1 - (mitad - _pixelActual), 0);
      _pixelActual++;
    }
    _strip.show();

    if (_pixelActual > mitad) {
      _animando = false;
      _strip.clear();
      _strip.show();
    }
  }
}

// ─── Helpers ────────────────────────────────────────────────────────────────
bool SableLED::estaAnimando() {
  return _animando;
}

void SableLED::setVelocidadApagado(uint8_t velocidad) {
  _velocidadApagado = velocidad;
}
