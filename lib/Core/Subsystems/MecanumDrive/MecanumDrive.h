#pragma once
#include <Arduino.h>
#include "Components/Motor/Motor.h"
#include "Tools/Localizer.h"

class MecanumDrive {
public:
  MecanumDrive(Motor& _frontLeft, Motor& _frontRight, Motor& _backLeft, Motor& _backRight);

  void begin(); // inicia los 4 motores y sus encoders
  void update(); // actualiza la velocidad de los 4 encoders

  void forward(int speed, bool forwardDirection = true); // true es adelante
  void forwardBy(int speed, float distance);
  void strafe(int speed, bool rightDirection = true); // true es a la derecha
  void strafeBy(int speed, float distance);
  void stop();

private:
  Motor& frontLeft;
  Motor& frontRight;
  Motor& backLeft;
  Motor& backRight;

  //Localizer localizer;

};