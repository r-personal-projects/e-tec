#define LEDAnfang 5 //erster Pin
#define LEDEnde 10 //Letzter Pin
#define Schrittweite 1 //Wie viele Pins entfernt?
#define Wartezeit 5
/*
Name:		Projekt2_1.ino
Created:	03.04.2016 10:48:39
Author:	Rubeen
*/

// Methode beim Start
void setup()
{
	//solange, wie...
	//Setze i = LEDAnfang
	//... i kleiner o. gleich LEDEnde ist,
	//addiere Schrittweite
	for (int i = LEDAnfang; i <= LEDEnde; i += Schrittweite)
	{
		pinMode(i, OUTPUT);
	}
}

// Schleife während Betrieb
void loop()
{
	for (int i = 0; i <= 255; i++)
	{
		for (int j = LEDAnfang; j <= LEDEnde; j = j + Schrittweite)
		{
			analogWrite(j, i);
		}
		delay(Wartezeit * 2);
	}

	for (int j = LEDAnfang; j <= LEDEnde; j = j + Schrittweite)
	{
		for (int i = 255; i >= 0; i--)
		{
			analogWrite(j, i);
			delay(Wartezeit / 2);
		}
	}
}
