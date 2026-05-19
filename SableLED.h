#ifndef SABLE_LED_H
#define SABLE_LED_H

#include <Adafruit_NeoPixel.h>

class SableLED {
  public:
    SableLED(uint8_t pin, uint16_t numLeds, uint8_t velocidad);
    void begin();
    void update();
    void toggleEncendido();
    bool estaEncendido();
    void setColor(uint32_t color);
    void golpe();
    bool estaAnimando();
    void setVelocidadApagado(uint8_t velocidad);
  private:
    Adafruit_NeoPixel _strip;
    uint32_t _color;
    uint8_t  _velocidad;
    bool     _encendido;
    bool     _animando;       // ← NUEVO
    bool     _encendiendose;  // ← NUEVO
    int      _pixelActual;    // ← NUEVO
    uint32_t _ultimoPaso;     // ← NUEVO
    void encender();
    void apagar();
    uint8_t _velocidadApagado;
};

#endif
