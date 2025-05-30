void function5()
{
Serial.print("Function: 5");
strip.clear();
//int localBrightness = 20;
//`strip.setBrightness(localBrightness);
for (int i = 0; i < 200; i++) 
  {
    strip.setPixelColor(i, strip.Color(220, 20, 60)); 
    strip.show();
    delay(20);
  }
for (int i = 0; i < 200; i++) 
{

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Moderately bright green color.
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(20);
}
for (int i = 0; i < 200; i++) 
  {
    strip.setPixelColor(i, strip.Color(220, 20, 60)); 
    strip.show();
    delay(20);
  }
for (int i = 0; i < 200; i++) 
{

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Moderately bright green color.
    strip.show(); // This sends the updated pixel color to the hardware.
    delay(20);
}
}
