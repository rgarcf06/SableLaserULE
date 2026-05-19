#include "ColorSable.h"

const uint32_t ColorSable::_colores[NUM_COLORES] = {
  Adafruit_NeoPixel::Color(0,   0,   255), // Azul
  Adafruit_NeoPixel::Color(0,   255,   0), // Verde
  Adafruit_NeoPixel::Color(255,   0,   0), // Rojo
  Adafruit_NeoPixel::Color(255,   0, 255), // Morado
  Adafruit_NeoPixel::Color(255, 255, 255), // Blanco
  Adafruit_NeoPixel::Color(255, 165,   0), // Naranja
  Adafruit_NeoPixel::Color(0,   255, 255), // Cian
  Adafruit_NeoPixel::Color(255, 255,   0), // Amarillo
};

const char* ColorSable::_nombres[NUM_COLORES] = {
  "Azul    - Jedi",
  "Verde   - Jedi",
  "Rojo    - Sith",
  "Morado  - Mace Windu",
  "Blanco  - Rey",
  "Naranja - Mandalorian",
  "Cian    - Cal Kestis",
  "Amarillo- Guardias Jedi",
};

ColorSable::ColorSable() : _colorActual(0) {}

void ColorSable::siguiente() {
  _colorActual = (_colorActual + 1) % NUM_COLORES;
}

uint32_t ColorSable::getColor() {
  return _colores[_colorActual];
}

const char* ColorSable::getNombre() {
  return _nombres[_colorActual];
}