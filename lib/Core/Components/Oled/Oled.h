#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "Config.h"

class Oled {
public:
  Oled(uint8_t _width = Config::OLED_WIDTH, uint8_t _height = Config::OLED_HEIGHT);

  bool begin(uint8_t addr = Config::OLED_ADDR);

  void clear();                                                  // borra la pantalla

  // Borra todo y escribe el texto desde (x, y). size = tamaño de letra (1, 2, 3...)
  void showText(const String& text, uint8_t size = 1, int16_t x = 0, int16_t y = 0);

  // Escribe en una línea (0, 1, 2...) sin borrar las demás
  void showLine(uint8_t line, const String& text, uint8_t size = 1);

private:
  uint8_t width, height;
  Adafruit_SSD1306 display;
};