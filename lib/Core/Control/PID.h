#pragma once
#include <Arduino.h>

// Controlador PID para velocidad de motores y para el rumbo (yaw) del chasis.
//  - dt real calculado con micros()
//  - Derivada sobre la medición (sin "patada" al cambiar el setpoint)
//  integral se limita al rango de salida
//  el error se envuelve a +-180 grados
class PID {
public:
  PID(float _kp = 0, float _ki = 0, float _kd = 0,
      float _outMin = -100, float _outMax = 100)
    : kp(_kp), ki(_ki), kd(_kd), outMin(_outMin), outMax(_outMax) {}

  void setGains(float _kp, float _ki, float _kd) { kp = _kp; ki = _ki; kd = _kd; }
  void setOutputLimits(float _outMin, float _outMax) { outMin = _outMin; outMax = _outMax; }
  void setAngular(bool _angular) { angular = _angular; }   // true para yaw

  // Llamar al empezar un movimiento nuevo o tras una pausa larga
  void reset() { integral = 0; first = true; }

  // Usa el tiempo real entre llamadas
  float compute(float setpoint, float measurement) {
    uint32_t now = micros();
    float dt = first ? 0.0f : (now - lastUs) * 1e-6f;
    lastUs = now;
    return compute(setpoint, measurement, dt);
  }

  // Usa un dt dado (en segundos)
  float compute(float setpoint, float measurement, float dt) {
    float error = wrap(setpoint - measurement);
    float derivative = 0;

    if (!first && dt > 0) {
      integral += ki * error * dt;
      integral = constrain(integral, outMin, outMax); // clamp integral
      derivative = -wrap(measurement - lastMeasurement) / dt;  // derivada
    }
    first = false;
    lastMeasurement = measurement;

    float out = kp * error + integral + kd * derivative;
    return constrain(out, outMin, outMax);
  }

  float getIntegral() const { return integral; }

private:
  float wrap(float a) const { // wraper angle
    if (!angular) return a;
    while (a >  180) a -= 360;
    while (a < -180) a += 360;
    return a;
  }

  float kp, ki, kd;
  float outMin, outMax;
  bool  angular = false;

  float    integral = 0;
  float    lastMeasurement = 0;
  uint32_t lastUs = 0;
  bool     first = true;
};