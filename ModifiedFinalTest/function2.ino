void function2()
{

    Serial.print("Function: 2");
    rainbow(0); 
}

void rainbow(int wait) {
  /** Hue of first pixel runs 5 complete loops through the color wheel.
   Color wheel has a range of 65536 but it's fine if we roll over, so
   just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
   means we'll make 5*65536/256 = 1280 passes through this outer loop: */
  for(long firstPixelHue = 0; firstPixelHue < 2*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      /**Offset pixel hue by an amount to make one full revolution of the
       color wheel (range of 65536) along the length of the strip
       (strip.numPixels() steps):*/
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      /** strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
       before assigning to each pixel: */
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}
