#pragma once
#include <Arduino.h>
#include <7Semi_BMI270.h>
#include "Pins.h"
#include "Config.h"


class Imu {
public:
  bool  begin(uint8_t _addr = Config::IMU_ADDR,
              int _sda = Pins::I2C_SDA,
              int _scl = Pins::I2C_SCL);
  void  update();                        // llamar seguido (cada ~5 a 10 ms)

  bool  calibrateGyro(int n = 300);      // el robot debe estar quieto
  void  resetYaw();                      // el yaw actual pasa a ser 0

  float getYaw()   const { return yawNorm; }
  float getRoll()  const { return roll; }
  float getPitch() const { return pitch; }

private:
  static constexpr float ALPHA   = 0.98f;      // 98% giroscopio, 2% acelerómetro
  static constexpr float RAD2DEG = 57.29578f;

  BMI270_7Semi imu;

  float roll = 0, pitch = 0, yaw = 0;
  float yawNorm = 0;
  float yawOffset = 0;
  float gbx = 0, gby = 0, gbz = 0;    // bias del giroscopio (°/s)
  unsigned long lastUs = 0;
};