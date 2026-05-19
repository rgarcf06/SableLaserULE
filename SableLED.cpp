#include "SableLED.h"

SableLED::SableLED(uint8_t pin, uint16_t numLeds, uint8_t velocidad)
  : _strip(numLeds, pin, NEO_GRB + NEO_KHZ800),
    _color(Adafruit_NeoPixel::Color(0, 0, 255)), // Azul por defecto
    _velocidad(velocidad),
    _velocidadApagado(velocidad),
    _encendido(false) {}

void SableLED::begin() {
  _strip.begin();
  _strip.setBrightness(150);
  _strip.clear();
  _strip.show();
  delay(100);
}

void SableLED::toggleEncendido() {
  _encendido    = !_encendido;
  _encendiendose = _encendido;
  _animando     = true;
  _pixelActual  = 0;
  _ultimoPaso   = millis();

}

bool SableLED::estaEncendido() {
  return _encendido;
}

void SableLED::setColor(uint32_t color) {
  _color = color;
  if (_encendido) {
    _strip.fill(_color);
    _strip.show();
  }
}

void SableLED::encender() {
  int mitad = _strip.numPixels() / 2;
  for (int i = 0; i < mitad; i++) {
    _strip.setPixelColor(i, _color);
    _strip.setPixelColor(_strip.numPixels() - 1 - i, _color);
    _strip.show();
    delay(_velocidad);
  }
  if (_strip.numPixels() % 2 != 0) {
    _strip.setPixelColor(mitad, _color);
    _strip.show();
  }
}

void SableLED::apagar() {
  int mitad = _strip.numPixels() / 2;
  if (_strip.numPixels() % 2 != 0) {
    _strip.setPixelColor(mitad, 0);
    _strip.show();
    delay(_velocidad);
  }
  for (int i = mitad - 1; i >= 0; i--) {
    _strip.setPixelColor(i, 0);
    _strip.setPixelColor(_strip.numPixels() - 1 - i, 0);
    _strip.show();
    delay(_velocidad);
  }
}

void SableLED::golpe() {
  if (!_encendido) return;

  // Flash blanco
  _strip.fill(Adafruit_NeoPixel::Color(255, 255, 255));
  _strip.setBrightness(255);
  _strip.show();
  delay(80);

  // Segundo flash más suave
  _strip.setBrightness(180);
  _strip.show();
  delay(60);

  // Volver al color y brillo original
  _strip.fill(_color);
  _strip.setBrightness(150);
  _strip.show();
}

void SableLED::update() {
  if (!_animando) return;

  uint32_t ahora = millis();
  if ((ahora - _ultimoPaso) < _velocidad) return;
  _ultimoPaso = ahora;

  int mitad = _strip.numPixels() / 2;

  if (_encendiendose) {
    if (_pixelActual <= mitad) {
      _strip.setPixelColor(_pixelActual, _color);
      _strip.setPixelColor(_strip.numPixels() - 1 - _pixelActual, _color);
      _strip.show();
      _pixelActual++;
    } else {
      _animando = false;
    }
  } else {
    if (_pixelActual <= mitad) {
      _strip.setPixelColor(mitad - _pixelActual, 0);
      _strip.setPixelColor(_strip.numPixels() - 1 - (mitad - _pixelActual), 0);
      _strip.show();
      _pixelActual++;
    } else {
      _animando = false;
    }
  }
}

bool SableLED::estaAnimando() {
  return _animando;
}

void SableLED::setVelocidadApagado(uint8_t velocidad) {
  _velocidadApagado = velocidad;
}
