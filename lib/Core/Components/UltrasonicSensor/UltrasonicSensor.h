#pragma once
#include <Arduino.h>

class UltrasonicSensor {
public:
  UltrasonicSensor(uint8_t _trigPin, uint8_t _echoPin);

  void  begin();

  // Distancia en cm. Devuelve -1 si no hubo eco (nada enfrente o fuera de rango).
  float getDistance();

private:
  static constexpr float SOUND_SPEED = 0.0343f;   // cm/us
  static constexpr unsigned long TIMEOUT_US  = 25000;     // ~4 m; evita bloquear el loop

  const uint8_t trigPin;
  const uint8_t echoPin;
};