#include "Imu.h"

bool Imu::calibrateGyro(int n) {
  float sx = 0, sy = 0, sz = 0;
  int ok = 0, tries = 0;
  while (ok < n && tries < n * 5) {          // límite para no colgarse
    float gx, gy, gz;
    if (imu.readGyro(gx, gy, gz)) { sx += gx; sy += gy; sz += gz; ok++; }
    tries++;
    delay(5);
  }
  if (ok < n) return false;
  gbx = sx / n; gby = sy / n; gbz = sz / n;
  return true;
}

void Imu::resetYaw() {
  yawOffset = yaw;
}

bool Imu::begin(uint8_t addr, int sda, int scl) {
  BMI270_7Semi::Config cfg;
  cfg.bus   = BMI270_7Semi::Bus::I2C;
  cfg.addr  = addr;
  cfg.sda   = sda;
  cfg.scl   = scl;
  cfg.i2cHz = 400000;

  if (!imu.begin(cfg)) {
    Serial.println("ERROR: BMI270 initialization failed!");
    return false;
  }

  imu.setAccelConfig(BMI2_ACC_ODR_100HZ, BMI2_ACC_RANGE_2G,
                      BMI2_ACC_NORMAL_AVG4, BMI2_PERF_OPT_MODE);
  imu.setGyroConfig(BMI2_GYR_ODR_100HZ, BMI2_GYR_RANGE_500,   // 500 °/s: mejor resolución
                     BMI2_GYR_NORMAL_MODE, BMI2_PERF_OPT_MODE);

  Serial.println("Calibrando giroscopio...");
  delay(500);
  if (!calibrateGyro()) return false;

  float ax, ay, az;
  if (imu.readAccel(ax, ay, az)) {
    roll  = atan2f(ay, az) * RAD2DEG;
    pitch = atan2f(-ax, sqrtf(ay * ay + az * az)) * RAD2DEG;
  }
  lastUs = micros();
  Serial.println("IMU inicializado");
  return true;
}

void Imu::update() {
  float ax, ay, az, gx, gy, gz;
  if (!imu.readAccel(ax, ay, az) || !imu.readGyro(gx, gy, gz)) return;

  // dt real entre muestras
  unsigned long now = micros();
  float dt = (now - lastUs) * 1e-6f;
  lastUs = now;
  if (dt <= 0 || dt > 0.1f) dt = 0.01f;      // protección ante saltos

  // Quita el bias
  gx -= gbx; gy -= gby; gz -= gbz;

  // Ángulos según el acelerómetro
  float accRoll  = atan2f(ay, az) * RAD2DEG;
  float accPitch = atan2f(-ax, sqrtf(ay * ay + az * az)) * RAD2DEG;

  // Filtro complementario
  roll  = ALPHA * (roll  + gx * dt) + (1.0f - ALPHA) * accRoll;
  pitch = ALPHA * (pitch + gy * dt) + (1.0f - ALPHA) * accPitch;

  // Yaw: solo giroscopio (con drift)
  yaw += gz * dt;
  if (yaw >  180) yaw -= 360;
  if (yaw < -180) yaw += 360;

  yawNorm = yaw - yawOffset;
  if (yawNorm >  180) yawNorm -= 360;
  if (yawNorm < -180) yawNorm += 360;
}
