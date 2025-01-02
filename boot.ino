#include "icons.h"
#include "functions.h"
const String name = "ESP8266 Custom";
const String name2 = "Deauther";
const String github = "by @V3n0m1010011010";
void boot() {
  dis.clearDisplay();
  dis.setTextSize(1);
  dis.setTextColor(SH110X_WHITE);
  dis.drawBitmap(128 / 4, 0, logo, 64, 64, 1);
  dis.display();
  delay(1500);
  dis.clearDisplay();
  dis.setCursor(getCenter(name.c_str()), 10);
  dis.println(name);
  dis.setCursor(getCenter(name2.c_str()), 20);
  dis.println(name2);
  dis.setCursor(getCenter(github.c_str()), 40);
  dis.println(github);
  dis.display();
  delay(2000);
}