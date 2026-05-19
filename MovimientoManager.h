#pragma once
#include <DFRobot_BMI160.h>
#include <Wire.h>

#define UMBRAL_MOVIMIENTO  4000
#define UMBRAL_GOLPE      17000

enum EstadoIMU { IMU_REPOSO, IMU_MOVIMIENTO, IMU_GOLPE };

class MovimientoManager {
public:
  MovimientoManager(uint8_t sda, uint8_t scl);
  bool begin();
  void leer();
  EstadoIMU getEstado();

private:
  DFRobot_BMI160 _bmi160;
  uint8_t        _sda;
  uint8_t        _scl;
  int16_t        _accelPrev[3];
  EstadoIMU      _estado;
};