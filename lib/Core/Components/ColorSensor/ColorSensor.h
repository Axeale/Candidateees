#pragma once
#include <Arduino.h>
#include <Adafruit_TCS34725.h>


class ColorSensor {
public:
  ColorSensor(uint8_t _integration = TCS34725_INTEGRATIONTIME_300MS,
              tcs34725Gain_t _gain = TCS34725_GAIN_1X);

  bool   begin();
  void   update();    // OJO: bloquea ~ el tiempo de integración (300 ms por defecto)

  // Valores normalizados contra Clear (0 a 255), como en tu sketch
  int      getRed()       const { return redClean; }
  int      getGreen()     const { return greenClean; }
  int      getBlue()      const { return blueClean; }
  uint16_t getClear()     const { return c; }
  uint16_t getColorTemp() const { return colorTemp; }
  uint16_t getLux()       const { return lux; }

  String getColor() const;                                        // clasifica la última lectura
  String findColor(int red_255, int green_255, int blue_255, float c) const;
  static bool inRange(float var, float min, float max);

private:
  Adafruit_TCS34725 tcs;

  uint16_t r = 0, g = 0, b = 0, c = 0;
  uint16_t colorTemp = 0, lux = 0;
  int redClean = 0, greenClean = 0, blueClean = 0;
};