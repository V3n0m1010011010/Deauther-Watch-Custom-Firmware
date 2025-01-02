#include "functions.h"
uint16_t getCenter(const char* string) {
  extern Adafruit_SH1106G dis;
  int16_t x1, y1;
  uint16_t textWidth, textHeight;
  dis.getTextBounds(string, 0, 0, &x1, &y1, &textWidth, &textHeight);
  uint16_t center = (128 - textWidth) / 2;
  return center;
}