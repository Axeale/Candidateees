#include "ServoPCA.h"

ServoPCA::ServoPCA(Adafruit_PWMServoDriver& _pca, uint8_t _channel, float _pwmFreqHz)
  : pca(_pca), channel(_channel), freq(_pwmFreqHz) {}

void ServoPCA::begin(float startAngle) {
  if (freq > 400.0f) {
    Serial.printf("AVISO: el PCA9685 está a %.0f Hz; un servo necesita ~50 a 300 Hz\n", freq);
  }
  write(startAngle);
}

void ServoPCA::setPulseRange(uint16_t minUs, uint16_t maxUs) {
  minUs = minUs;
  maxUs = maxUs;
}

void ServoPCA::write(float angle) {
  angle = constrain(angle, 0.0f, 180.0f);
  uint16_t us = minUs + (uint16_t)((maxUs - minUs) * (angle / 180.0f));
  writeMicroseconds(us);
}

void ServoPCA::writeMicroseconds(uint16_t us) {
  // ticks = duración del pulso / duración de un tick (periodo / 4096)
  float ticks = us * freq * 4096.0f / 1000000.0f;
  uint16_t t = (uint16_t)constrain(ticks + 0.5f, 0.0f, 4095.0f);
  pca.setPWM(channel, 0, t);
}

void ServoPCA::detach() {
  pca.setPWM(channel, 0, 0);
}