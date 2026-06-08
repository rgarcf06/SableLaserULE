#include "SableLED.h"

#define LED_INICIO 10
#define LED_FIN 310

// ─── Constructor ────────────────────────────────────────────────────────────
SableLED::SableLED(uint8_t pin, uint16_t numLeds, uint8_t velocidad, uint8_t offset)
  : _strip(numLeds, pin, NEO_GRB + NEO_KHZ800),
    _color(Adafruit_NeoPixel::Color(0, 0, 255)),
    _velocidad(velocidad),
    _velocidadApagado(velocidad),
    _offset(offset),
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
    _strip.fill(_color, LED_INICIO);
    _strip.show();
  }
}

// ─── Golpe (no bloqueante) ──────────────────────────────────────────────────
void SableLED::golpe() {
  if (!_encendido) return;

  _enGolpe     = true;
  _animando    = true;
  _tiempoGolpe = millis();

  _strip.fill(Adafruit_NeoPixel::Color(255, 255, 255), LED_INICIO);
  _strip.setBrightness(200);
  _strip.show();
}

// ─── Update (llamar en cada loop) ───────────────────────────────────────────
void SableLED::update() {

  // — Resolución del golpe —
  if (_enGolpe) {
    if ((millis() - _tiempoGolpe) > 140) {
      _enGolpe  = false;
      _animando = false;
      _strip.fill(_color, LED_INICIO);
      _strip.setBrightness(150);
      _strip.show();
    }
    return;
  }

  // — Animación encendido / apagado —
  if (!_animando) return;

  uint32_t ahora = millis();
  if ((ahora - _ultimoPaso) < _velocidad) return;
  _ultimoPaso = ahora;

  int ladoIda    = _strip.numPixels() / 2 - LED_INICIO; // 150 LEDs (10→159)
  int ladoVuelta = _strip.numPixels() / 2;               // 160 LEDs (160→319)
  int paso       = 4;

  if (_encendiendose) {
    // Encendido: base→punta, ambos lados llegan a la punta a la vez
    // La vuelta marca el ritmo (más LEDs), la ida se escala con map()
    for (int i = 0; i < paso && _pixelActual < ladoVuelta; i++) {
      // Lado vuelta: 319, 318, 317 ... 160
      _strip.setPixelColor(LED_FIN - _pixelActual, _color);

      // Lado ida: escalado para llegar a la punta al mismo tiempo
      int posIda = map(_pixelActual, 0, ladoVuelta - 1, 0, ladoIda - 1);
      _strip.setPixelColor(LED_INICIO + posIda, _color);

      _pixelActual++;
    }
    _strip.show();

    if (_pixelActual >= ladoVuelta) {
      _animando = false;
    }

  } else {
    // Apagado: punta→base, ambos lados salen de la punta a la vez
    for (int i = 0; i < paso && _pixelActual < ladoVuelta; i++) {
      // Lado vuelta: 160, 161, 162 ... 319
      _strip.setPixelColor(_strip.numPixels() / 2 + _pixelActual, 0);

      // Lado ida: escalado
      int posIda = map(_pixelActual, 0, ladoVuelta - 1, 0, ladoIda - 1);
      _strip.setPixelColor(_strip.numPixels() / 2 - 1 - posIda, 0);

      _pixelActual++;
    }
    _strip.show();

    if (_pixelActual >= ladoVuelta) {
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
