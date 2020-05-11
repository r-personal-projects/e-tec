#define ANZAHL_LEDS 6
#define PAUSE 500
#define ANZEIGEDAUER 200
const int LEDs[]{ 5, 7, 9, 6, 8, 10 }; //Array von LEDs

/*
Name:		Projekt2_1.ino
Created:	03.04.2016 10:48:39
Author:	Rubeen
*/

// Methode beim Start
void setup()
{
	//solange, wie...
	//Setze i = 0
	//... i kleiner als Anzahl der LEDs
	//addiere 1
	for (int i = 0; i <= 6; i ++)
	{
		pinMode(LEDs[i], OUTPUT);
	}
}

// Schleife während Betrieb
void loop()
{
	for (int i = 0; i < ANZAHL_LEDS; i++)
	{
		digitalWrite(LEDs[i], HIGH);
		delay(ANZEIGEDAUER);
	}

	delay(PAUSE);

	for (int i = ANZAHL_LEDS - 1; i >= 0; i--)
	{
		digitalWrite(LEDs[i], LOW);
		delay(ANZEIGEDAUER);
	}

	delay(PAUSE);
}