#include "Oled.h"

Oled::Oled(uint8_t _width, uint8_t  _height)
  : width(_width), height(_height), display(_width, _height, &Wire, -1) {}

bool Oled::begin(uint8_t addr) {
  if (!display.begin(SSD1306_SWITCHCAPVCC, addr)) {
    Serial.println("ERROR: no se encontró la pantalla OLED");
    return false;
  }
  display.setTextColor(SSD1306_WHITE);
  display.clearDisplay();
  display.display();
  return true;
}

void Oled::clear() {
  display.clearDisplay();
  display.display();
}

void Oled::showText(const String& text, uint8_t size, int16_t x, int16_t y) {
  display.clearDisplay();
  display.setTextSize(size);
  display.setCursor(x, y);
  display.print(text);
  display.display();
}

void Oled::showLine(uint8_t line, const String& text, uint8_t size) {
  int16_t h = 8 * size;
  int16_t y = line * h;
  if (y >=  height) return;

  display.fillRect(0, y, width, h, SSD1306_BLACK);   // borra solo esa línea
  display.setTextSize(size);
  display.setCursor(0, y);
  display.print(text);
  display.display();
}