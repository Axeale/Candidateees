#include "MecanumDrive.h"

MecanumDrive::MecanumDrive(Motor& frontLeft, Motor& frontRight, Motor& backLeft, Motor& backRight)
  : frontLeft(frontLeft), frontRight(frontRight),
    backLeft(backLeft), backRight(backRight) {}

void MecanumDrive::begin() {
  frontLeft.begin();
  frontRight.begin();
  backLeft.begin();
  backRight.begin();
}

void MecanumDrive::update() {
  frontLeft.update();
  frontRight.update();
  backLeft.update();
  backRight.update();
}

void MecanumDrive::forward(int speed, bool forwardDirection) {
  int s = forwardDirection ? speed : -speed;
  frontLeft.setSpeed(s);
  frontRight.setSpeed(s);
  backLeft.setSpeed(s);
  backRight.setSpeed(s);
}

void MecanumDrive::forwardBy(int speed, float distance) {
    if (distance < 0){
        distance = -distance;
        speed = -speed;
    }

    while (true) {
        update();
        float avgDistance = 0;
        avgDistance += (frontLeft.getCount() + frontRight.getCount() + backLeft.getCount() + backRight.getCount()) / 4.0;

        if (avgDistance >= distance) { // Es posible que aqui haya problema porque no estoy seguro como es el .getcouont() y ademas el avg distance se sta sumando en ambos casos de distancia negativa y positiva
            break; // por lo que el encoder se supoee que debeira ir invertido hacia el otro lado
        }
        forward(speed);
    }
}
void MecanumDrive::strafe(int speed, bool rightDirection) {
  int s = rightDirection ? speed : -speed;
  frontLeft.setSpeed(s);
  frontRight.setSpeed(-s);
  backLeft.setSpeed(-s);
  backRight.setSpeed(s);
}

void MecanumDrive::stop() {
  frontLeft.stop();
  frontRight.stop();
  backLeft.stop();
  backRight.stop();
}