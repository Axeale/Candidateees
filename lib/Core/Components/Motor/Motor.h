#pragma once
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include <ESP32Encoder.h>

// Un motor con DRV8833 (dos canales del PCA9685) y su encoder en cuadratura.
class Motor {
public:
  Motor(Adafruit_PWMServoDriver& _pca,
        uint8_t _pin1, uint8_t _pin2,        // canales del PCA9685
        uint8_t _encA, uint8_t _encB,        // GPIO del encoder
        bool _reversed = false);

  // El encoder tiene un recurso de hardware (PCNT): no se puede copiar.
  Motor(const Motor&) = delete;
  Motor& operator=(const Motor&) = delete;

  void  begin();                  // llamar DESPUÉS de pca.begin()
  void  update();                 // llamar en cada loop (calcula la velocidad del encoder)

  void  setSpeed(int vel);        // -100 a 100
  void  stop();
  void  setReversed(bool reversed);
  void  test();                   // barrido de velocidades (bloqueante, solo para pruebas)

  long  getCount();               // cuentas acumuladas (positivo = hacia adelante)
  float getSpeed();               // cuentas por segundo
  void  resetCount();
  void  setEncoderInverted(bool inverted);   // si el signo del encoder sale al revés

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
};