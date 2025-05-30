
void function1()
{
  Serial.print("Function: 1");
  //int localBrightness = 20;
  //strip.setBrightness(localBrightness);
  for (int i = 0; i < 200; i++) 
  {
  matrix.setRotation(3);
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  scroll(Text[mode],15);                                                
  }
  
}


void scroll(char* message,int delays) 
{
  maxDisplacement = strlen(message) * pixelPerChar + matrix.width();
  if(++line_pass > matrix.width()) line_pass = 0;
  matrix.print(String(message));
    if(--x < -maxDisplacement) 
    {
      x = matrix.width();
      
    }
  matrix.show();
  delay(60); 
}
