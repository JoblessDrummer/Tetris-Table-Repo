void function3()
{
Serial.print("Function: 3");
strip.clear();
//int localBrightness = 20;
//strip.setBrightness(localBrightness);
for(int i = 0; i < 200; i += 2)
{
   strip.setPixelColor(i, strip.Color(220, 20, 60));
   strip.setPixelColor(i-1, strip.Color(35, 235, 195));
}
strip.show();
}


  
