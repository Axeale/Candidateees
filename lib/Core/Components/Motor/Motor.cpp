#include "Motor.h"

Motor::Motor(Adafruit_PWMServoDriver& _pca,
             uint8_t _pin1, uint8_t _pin2,
             uint8_t _encA, uint8_t _encB,
             bool _reversed)
  : pca(_pca), pin1(_pin1), pin2(_pin2),
    encA(_encA), encB(_encB), reversed(_reversed) {}

void Motor::begin() {
  ESP32Encoder::useInternalWeakPullResistors = puType::up;
  encoder.attachFullQuad(encA, encB);
  encoder.setFilter(1023); // filtro de ruido
  encoder.clearCount();

  lastCount = 0;
  speedCps  = 0;
  lastMs = millis();

  stop();
  Serial.printf("Motor listo (PCA %u,%u | Enc %u,%u)\n", pin1, pin2, encA, encB);
}

void Motor::update() {
  uint32_t now  = millis();
  uint32_t dtMs = now - lastMs;
  if (dtMs < SPEED_PERIOD_MS) return;

  long c = getCount();
  speedCps  = (c - lastCount) * 1000.0f / dtMs;
  lastCount = c;
  lastMs = now;
}

void Motor::setSpeed(int vel) {
  vel = constrain(vel, -100, 100);

  // Inversión física si el motor está montado al revés
  if (reversed) vel = -vel;

  uint16_t out = map(abs(vel), 0, 100, 0, 4095);

  if (vel == 0) {
    pca.setPWM(pin1, 0, 0);
    pca.setPWM(pin2, 0, 0);
  } else if (vel > 0) {
    pca.setPWM(pin2, 0, 0);
    pca.setPWM(pin1, 0, out);
  } else {
    pca.setPWM(pin1, 0, 0);
    pca.setPWM(pin2, 0, out);
  }
}

void Motor::stop() {
  pca.setPWM(pin1, 0, 0);
  pca.setPWM(pin2, 0, 0);
}

void Motor::setReversed(bool reversed) {
  reversed = reversed;
}

void Motor::setEncoderInverted(bool inverted) {
  encInverted = inverted;
}

long Motor::getCount() {
  long c = (long)encoder.getCount();
  if (reversed != encInverted) c = -c;
  return c;
}

float Motor::getSpeed() {
  return speedCps;
}

void Motor::resetCount() {
  encoder.clearCount();
  lastCount = 0;
}

void Motor::test() {
  for (int vel = 0; vel < 100; vel += 10) {
    Serial.print("Vel:"); Serial.println(vel);
    setSpeed(vel);
    delay(100);
  }
  for (int vel = 100; vel > -100; vel -= 10) {
    Serial.print("Vel:"); Serial.println(vel);
    setSpeed(vel);
    delay(100);
  }
  for (int vel = -100; vel < 0; vel += 10) {
    Serial.print("Vel:"); Serial.println(vel);
    setSpeed(vel);
    delay(100);
  }
  stop();
}