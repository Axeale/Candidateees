#pragma once
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <ESP32Encoder.h>
#include "Control/PID.h"

// Un motor con DRV8833 (dos canales del PCA9685) y su encoder en cuadratura.
class Motor {
public:
  Motor(Adafruit_PWMServoDriver& _pca,
        uint8_t _pin1, uint8_t _pin2, // canales del PCA9685
        uint8_t _encA, uint8_t _encB, // GPIO del encoder
        bool _reversed = false);

  // El encoder tiene un recurso de hardware (PCNT): no se puede copiar.
  Motor(const Motor&) = delete;
  Motor& operator=(const Motor&) = delete;

  void begin(); // llamar DESPUÉS de pca.begin()
  void update(); // llamar en cada loop (calcula la velocidad del encoder)

  void setSpeed(int vel); // -100 a 100
  void setTargetSpeed(int vel); // -100 a 100
  void stop();

  void setPID(float kp, float ki, float kd) { pid.setGains(kp, ki, kd); }
  void setMaxCps(float maxCps) { kF = (maxCps > 0) ? 100.0f / maxCps : 0; }

  void setReversed(bool reversed);
  void test(); 

  long getCount(); // ticks acumulados (positivo = hacia adelante)
  float getSpeed(); // Ticks per second
  void resetCount();
  void setEncoderInverted(bool inverted);   // si el signo del encoder sale al revés

private:
  static constexpr uint32_t SPEED_PERIOD_MS = 50;

  Adafruit_PWMServoDriver& pca;
  uint8_t pin1, pin2;
  uint8_t encA, encB;
  bool    reversed;
  bool    encInverted = false;

  ESP32Encoder encoder;
  long     lastCount = 0;
  float    speedCps  = 0;
  uint32_t lastMs    = 0;

  PID pid;
  float targetSpeed = 0; // -100 a 100
  float kF = 0;         // factor de feedforward (100 / cps a
};