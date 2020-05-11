#define LEDAnfang 5 //erster Pin
#define LEDEnde 10 //Letzter Pin
#define Schrittweite 1 //Wie viele Pins entfernt?
#define Wartezeit 500 //Wie viele ms soll gewartet werden?
/*
 Name:		Projekt2.ino
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
	for (int i = LEDAnfang; i <= LEDEnde; i = i + Schrittweite)
	{
		pinMode(i, OUTPUT);
	}
}

// Schleife während Betrieb
void loop() 
{
	for (int i = LEDAnfang; i <= LEDEnde; i = i + Schrittweite)
	{
		digitalWrite(i, HIGH);
		delay(Wartezeit);
		digitalWrite(i, LOW);
	}
}
