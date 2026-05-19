#ifndef COLOR_SABLE_H
#define COLOR_SABLE_H

#include <Adafruit_NeoPixel.h>

class ColorSable {
  public:
    ColorSable();
    void siguiente();
    uint32_t getColor();
    const char* getNombre();

  private:
    int _colorActual;
    static const int NUM_COLORES = 8;
    static const uint32_t _colores[NUM_COLORES];
    static const char* _nombres[NUM_COLORES];
};

#endif