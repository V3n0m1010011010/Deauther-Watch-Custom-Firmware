void setLedAttack(){
  strip.setPixelColor(0, strip.Color(255, 0, 0));
  strip.show();
}
void setLedNormal(){
  strip.setPixelColor(0, strip.Color(0, 255, 0));
  strip.show();
}
void setLedMonitor(){
  strip.setPixelColor(0, strip.Color(0, 0, 255));
  strip.show();
}
void setLedScan(){
  strip.setPixelColor(0, strip.Color(150, 70, 255));
  strip.show();
}