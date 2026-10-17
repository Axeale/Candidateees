#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "Pins.h"
#include "Config.h"
#include "Components/Motor/Motor.h"
#include "Components/Imu/Imu.h"
#include "Components/ColorSensor/ColorSensor.h"
#include "Components/Servo/Servo.h"
#include "Components/UltrasonicSensor/UltrasonicSensor.h"
#include "Components/Oled/Oled.h"

#include "Subsystems/MecanumDrive/MecanumDrive.h"

// ---- Hardware compartido ----
Adafruit_PWMServoDriver pca(Config::PCA_ADDR);

// ---- Motores (pin1, pin2 del PCA | encoder A, B | invertido) ----  
Motor FLMotor(pca, Pins::PCA_FLMOTOR[0], Pins::PCA_FLMOTOR[1], Pins::ENC_FLMOTOR[0], Pins::ENC_FLMOTOR[1], false);
Motor FRMotor(pca, Pins::PCA_FRMOTOR[0], Pins::PCA_FRMOTOR[1], Pins::ENC_FRMOTOR[0], Pins::ENC_FRMOTOR[1], true);
Motor BLMotor(pca, Pins::PCA_BLMOTOR[0], Pins::PCA_BLMOTOR[1], Pins::ENC_BLMOTOR[0], Pins::ENC_BLMOTOR[1], false);
Motor BRMotor(pca, Pins::PCA_BRMOTOR[0], Pins::PCA_BRMOTOR[1], Pins::ENC_BRMOTOR[0], Pins::ENC_BRMOTOR[1], true);
MecanumDrive chassis(FLMotor, FRMotor, BLMotor, BRMotor);

// ---- Demás módulos ----
ServoPCA servo(pca);
Imu imu;
ColorSensor colorSensor;
UltrasonicSensor usFront(Pins::US_TRIG, Pins::US_ECHO_A);
UltrasonicSensor usSide(Pins::US_TRIG, Pins::US_ECHO_B);
Oled oled;

unsigned long lastReport = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  // El I2C se inicia UNA sola vez, aquí
  Wire.begin(Pins::I2C_SDA, Pins::I2C_SCL);

  pca.begin();
  pca.setPWMFreq(Config::PCA_FREQ_HZ);

  chassis.begin();
  servo.begin(90);
  usFront.begin();
  usSide.begin();

  oled.begin();
  oled.showText("Iniciando...", 1);

  if (!imu.begin())         Serial.println("IMU no disponible");
  if (!colorSensor.begin()) Serial.println("Sensor de color no disponible");

  oled.showText("Listo", 2);
}

void loop() {
  // Lo que debe correr en cada ciclo
  chassis.update();
  imu.update();

  // Lo lento o bloqueante, solo cada 500 ms
  if (millis() - lastReport >= 500) {
    lastReport = millis();

    colorSensor.update();                 // bloquea ~300 ms
    float dFront = usFront.getDistance();
    float dSide  = usSide.getDistance();

    oled.showLine(0, "Yaw: " + String(imu.getYaw(), 1));
    oled.showLine(1, "Color: " + colorSensor.getColor());
    oled.showLine(2, "Frente: " + String(dFront, 1) + " cm");
    oled.showLine(3, "Lado: " + String(dSide, 1) + " cm");
    oled.showLine(4, "Enc FL: " + String(FLMotor.getCount()));
  }

  delay(5);
}