#pragma once
#include <Arduino.h>

namespace Config {
    // I2C addresses
  constexpr uint8_t PCA_ADDR = 0x40;
  constexpr uint8_t IMU_ADDR = 0x68;
  constexpr uint8_t COLOR_SENSOR_ADDR = 0x29;

  
  constexpr uint8_t OLED_ADDR = 0x3C;
  constexpr uint8_t OLED_WIDTH = 128;
  constexpr uint8_t OLED_HEIGHT = 64;

  constexpr float PCA_FREQ_HZ = 50.0f; // frecuencia de PWM para servos y motores

  // ---- Mapa ----
  constexpr float TILE_LENGTH_CM = 30.0f;
}