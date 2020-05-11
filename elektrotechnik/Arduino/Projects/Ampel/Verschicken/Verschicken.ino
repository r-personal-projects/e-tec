#include <Wire.h>
void setup()
{
  Serial.begin(9600);
  Wire.begin();
}
void loop()
{
  //Eingabe lesen
  byte eingabe = Serial.read();
  if(eingabe != 255)
    Verschicken(eingabe, 4);
}

void Verschicken(byte nachricht, int device)
{
  byte richtig = 0;
  if(nachricht >= 49 && nachricht <= 57)
    richtig = 1;
  if(nachricht >= 65 && nachricht <= 90)
    richtig = 1;
  if(nachricht >= 97 && nachricht <= 122)
  {
    nachricht = nachricht - 32;
    richtig = 1;
  }
  if(richtig == 1)
  {
    Wire.beginTransmission(device);
    Wire.write(nachricht);
    Serial.print(nachricht);
    Serial.println(" wurde gesendet.");
    Wire.endTransmission();
  }
}
