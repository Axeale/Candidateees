#pragma once
#include <Arduino.h>

namespace Pins {

  constexpr uint8_t I2C_SDA = 8; // lo usa el OLED, IMU, PCA9685, Color sensor, 
  constexpr uint8_t I2C_SCL = 9;
  
  
  constexpr uint8_t PCA_FLMOTOR[2] = {2,3};
  constexpr uint8_t PCA_FRMOTOR[2] = {0,1};
  constexpr uint8_t PCA_BLMOTOR[2] = {12,13};
  constexpr uint8_t PCA_BRMOTOR[2] = {14,15};
  
  // encoders
  constexpr uint8_t ENC_FLMOTOR[2] = {1,2};
  constexpr uint8_t ENC_FRMOTOR[2] = {14,42};
  constexpr uint8_t ENC_BLMOTOR[2] = {41,40};
  constexpr uint8_t ENC_BRMOTOR[2] = {39,38};
  
  //Ultrasonic sensor 
  constexpr uint8_t US_TRIG = 3;    
  constexpr uint8_t US_ECHO_A = 46;
  constexpr uint8_t US_ECHO_B = 48;
  // Servo
  constexpr uint8_t PWM_SERVO = 19; 
}