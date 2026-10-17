#pragma once
#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>
#include "Pins.h"
#include "Config.h"

// Servo conectado a un canal del PCA9685.
// (Se llama ServoPCA para no chocar con la clase "Servo" de otras librerías.)
class ServoPCA {
public:
  ServoPCA(Adafruit_PWMServoDriver& _pca,
           uint8_t _channel = Pins::PWM_SERVO,
           float _pwmFreqHz = Config::PCA_FREQ_HZ);

  void  begin(float startAngle = 90);      

  void  write(float angle); // 0 a 180 grados
  void  writeMicroseconds(uint16_t us); // pulso directo
  void  detach();// deja de mandar pulso (el servo se queda en donde esta)

  float getAngle() const { return angle; }
  void  setPulseRange(uint16_t minUs, uint16_t maxUs);   // calibra los extremos

private:
  Adafruit_PWMServoDriver& pca;
  uint8_t  channel;
  float    freq;
  uint16_t minUs = 544;
  uint16_t maxUs = 2400;
  float    angle = 90;
};