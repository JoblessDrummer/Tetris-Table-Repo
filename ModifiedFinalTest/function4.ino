void function4()
{
   Serial.print("Function: 4");
   strip.clear();
//int localBrightness = 20;
//strip.setBrightness(localBrightness);
   for(int i = 0; i < 200; i++)
   {
      strip.setPixelColor(i, strip.Color(255, 0, 255));
      delay(10);
      strip.show();
   }
   for(int i = 0; i < 200; i += 7);
   {
      
      strip.setPixelColor(i, strip.Color(0, 182, 237));
   }

}
