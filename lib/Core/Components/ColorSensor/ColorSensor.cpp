#include "ColorSensor.h"

ColorSensor::ColorSensor(uint8_t _integration, tcs34725Gain_t _gain)
  : tcs(_integration, _gain) {}

bool ColorSensor::begin() {
  if (!tcs.begin()) {
    Serial.println("Eror incializando el sensor de color");
    return false;
  }
  Serial.println("Sensor de color listo");
  return true;
}

void ColorSensor::update() {
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  float total = c;
  if (total > 0) {
    redClean   = (int)((r / total) * 255);
    greenClean = (int)((g / total) * 255);
    blueClean  = (int)((b / total) * 255);
  }
}

bool ColorSensor::inRange(float var, float min, float max) {
  return var >= min && var <= max;
}

String ColorSensor::getColor() const {
  return findColor(redClean, greenClean, blueClean, c);
}

String ColorSensor::findColor(int red_255, int green_255, int blue_255, float c) const {
  String colorDetectado = "Desconocido";

  // 1. NEGRO: Si hay muy poca luz total (Clear bajo)
  /*if (c < 100) { 
    colorDetectado = "Negro";
  }*/
  // 1. ROJO: El rojo predomina por mucho sobre el verde y el azul
  if (red_255 > green_255 * 1.5 && red_255 > blue_255 * 1.5 && green_255 < 100 && c < 500) {
    colorDetectado = "Rojo";
  }
  // 2. NARANJA: Mucho rojo, algo de verde (mitad del rojo aprox) y muy poco azul
  else if (red_255 > 150 && green_255 > 50 && green_255 < 100 && blue_255 < 50 && c > 300) {
    colorDetectado = "Naranja";
  }
  // 3. AMARILLO: Rojo y Verde están muy altos y parejos, azul muy bajo
  else if (red_255 > 100 && green_255 > 75 && blue_255 < 55 && c > 950) {
    colorDetectado = "Amarillo";
  }
  // 4. VERDE: El verde es claramente el más alto de los tres
  else if (green_255 > red_255 * 1.2 && green_255 > blue_255 * 1.3 && c < 500) {
    colorDetectado = "Verde";
  }
  // 5. AZUL: El azul predomina por completo
  else if (blue_255 > red_255 * 1.25 && blue_255 > green_255 * 1.25) {
    colorDetectado = "Azul";
  }
  // 6. Rosa: Rojo y Azul altos, Verde muy castigado/bajo
  else if (inRange(red_255, 115, 130) && inRange(green_255, 50, 60) && inRange(blue_255, 70, 80)) {
    colorDetectado = "Rosa";
  }
  return colorDetectado;
}