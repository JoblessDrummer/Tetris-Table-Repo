void function6()
{
Serial.print("Function: 6");
//int localBrightness = 20;
//strip.setBrightness(localBrightness);
int repeats = 20;
const int r = 220;
const int g = 20;
const int b = 60;
  for (int i = 0; i < repeats; i++) 
  {
    // Fade in
    for (int newBrightness = 0; newBrightness <= 155; newBrightness += 5) {
      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(
          (255 * newBrightness) / 155,
          (255 * newBrightness) / 155,
          (0 * newBrightness) / 155));
      }
      strip.show();
      delay(10);
    }

    // Fade out
    for (int newBrightness = 255; newBrightness >= 0; newBrightness -= 5) {
      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(
          (255 * newBrightness) / 255,
          (255 * newBrightness) / 255,
          (0 * newBrightness) / 255));
      }
      strip.show();
      delay(10);
    }
  }
/**for (int i = 0; i < repeats; i++) 
  {
    // Fade in
    for (int newBrightness = 0; newBrightness <= 155; newBrightness += 5) {
      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(
          (255 * newBrightness) / 155,
          (255 * newBrightness) / 155,
          (255 * newBrightness) / 155));
      }
      strip.show();
      delay(10);
    }

    // Fade out
    for (int newBrightness = 255; newBrightness >= 0; newBrightness -= 5) {
      for (int i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(
          (255 * newBrightness) / 255,
          (255 * newBrightness) / 255,
          (255 * newBrightness) / 255));
      }
      strip.show();
      delay(10);
    }
  }*/


  
}
