#include "MovimientoManager.h"

MovimientoManager::MovimientoManager(uint8_t sda, uint8_t scl)
  : _sda(sda), _scl(scl), _estado(IMU_REPOSO) {
  _accelPrev[0] = 0;
  _accelPrev[1] = 0;
  _accelPrev[2] = 0;
}

bool MovimientoManager::begin() {
  Wire.begin(_sda, _scl);
  _bmi160.softReset();
  if (_bmi160.I2cInit(0x69) != BMI160_OK) {
    if (_bmi160.I2cInit(0x68) != BMI160_OK) return false;
  }
  return true;
}

void MovimientoManager::leer() {
  int16_t accelGyro[6] = {0};
  if (_bmi160.getAccelGyroData(accelGyro) != 0) return;

  int16_t gx = accelGyro[0];
  int16_t gy = accelGyro[1];
  int16_t gz = accelGyro[2];
  int16_t ax = accelGyro[3];
  int16_t ay = accelGyro[4];
  int16_t az = accelGyro[5];

  int16_t dax = abs(ax - _accelPrev[0]);
  int16_t day = abs(ay - _accelPrev[1]);
  int16_t daz = abs(az - _accelPrev[2]);

  bool accelAlto = (dax > UMBRAL_GOLPE || day > UMBRAL_GOLPE || daz > UMBRAL_GOLPE);
  bool giroAlto  = (abs(gx) > UMBRAL_MOVIMIENTO || abs(gy) > UMBRAL_MOVIMIENTO || abs(gz) > UMBRAL_MOVIMIENTO);

  if      (accelAlto && !giroAlto) _estado = IMU_GOLPE;
  else if (giroAlto)               _estado = IMU_MOVIMIENTO;
  else                             _estado = IMU_REPOSO;

  _accelPrev[0] = ax;
  _accelPrev[1] = ay;
  _accelPrev[2] = az;
}

EstadoIMU MovimientoManager::getEstado() {
  return _estado;
}