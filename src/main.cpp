#include <Arduino.h>
#include "Pins.h"
#include "Config.h"
#include "Subsystems/MecanumDrive/MecanumDrive.h"
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pca(Config::PCA_ADDR);

Motor FLMotor(pca, Pins::PCA_FLMOTOR[0], Pins::PCA_FLMOTOR[1], Pins::ENC_FLMOTOR[0], Pins::ENC_FLMOTOR[1], false);
Motor FRMotor(pca, Pins::PCA_FRMOTOR[0], Pins::PCA_FRMOTOR[1], Pins::ENC_FRMOTOR[0], Pins::ENC_FRMOTOR[1], true);
Motor BLMotor(pca, Pins::PCA_BLMOTOR[0], Pins::PCA_BLMOTOR[1], Pins::ENC_BLMOTOR[0], Pins::ENC_BLMOTOR[1], false);
Motor BRMotor(pca, Pins::PCA_BRMOTOR[0], Pins::PCA_BRMOTOR[1], Pins::ENC_BRMOTOR[0], Pins::ENC_BRMOTOR[1], true);

MecanumDrive chassis(FLMotor, FRMotor, BLMotor, BRMotor);

void setup() {
  Serial.begin(115200);

  Wire.begin(Pins::I2C_SDA, Pins::I2C_SCL);
  pca.begin();
  pca.setPWMFreq(Config::PCA_FREQ_HZ);

  chassis.begin();

}

void loop() {
  chassis.forward(50);
  delay(2000);
  chassis.strafe(50);
  delay(2000);
  chassis.stop();
  delay(2000);
  Serial.println("Recorrido hecho :)");
}

