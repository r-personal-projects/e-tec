#define LED1 9
#define LED2 10
#define LED3 8
#define LED4 7
#define LED5 5
#define LED6 6
#define Wartezeit 100

/*
 Name:		Projekt1.ino
 Created:	24.03.2016 18:31:01
 Author:	Rubeen
*/

int LEDs[] = { LED1, LED2, LED3, LED4, LED5, LED6 };

// Die Setupmethode wird einmal beim Start oder Reset des Arduinos ausgeführt
void setup() {
	//Stelle jeden Pin mit LED als einen Ausgang ein, damit Strom an- und ausgeschaltet werden kann.
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);
	pinMode(LED5, OUTPUT);
	pinMode(LED6, OUTPUT);
}

// Die Loopmethode wird ständig ausgeführt, also durchgehend wiederholt, bis der Arduino neustartet.
void loop() {
	//Schalte eine LED an, warte kurz und schalte diese dann wieder aus. Mache das mit allen 6 LEDs
	digitalWrite(LED1, HIGH); //Die LED wird angeschaltet (= 5V)
	delay(Wartezeit);		  //Warte x ms. (= mache nichts)
	digitalWrite(LED1, LOW);  //Die LED wird ausgeschaltet (= 0V)
	
	//Mache das jetzt mit allen 6 LEDs
	digitalWrite(LED2, HIGH);
	delay(Wartezeit);
	digitalWrite(LED2, LOW);

	digitalWrite(LED3, HIGH);
	delay(Wartezeit);
	digitalWrite(LED3, LOW);

	digitalWrite(LED4, HIGH);
	delay(Wartezeit);
	digitalWrite(LED4, LOW);

	digitalWrite(LED5, HIGH);
	delay(Wartezeit);
	digitalWrite(LED5, LOW);

	digitalWrite(LED6, HIGH);
	delay(Wartezeit);
	digitalWrite(LED6, LOW);

	//Warte eine bestimmte Zeit, damit eine Pause entsteht.
	delay(Wartezeit);

	//Durchläuft die LEDs von hinten nach vorne
	digitalWrite(LED6, HIGH);
	delay(Wartezeit);
	digitalWrite(LED6, LOW);

	digitalWrite(LED5, HIGH);
	delay(Wartezeit);
	digitalWrite(LED5, LOW);

	digitalWrite(LED4, HIGH);
	delay(Wartezeit);
	digitalWrite(LED4, LOW);

	digitalWrite(LED3, HIGH);
	delay(Wartezeit);
	digitalWrite(LED3, LOW);

	digitalWrite(LED2, HIGH);
	delay(Wartezeit);
	digitalWrite(LED2, LOW);

	digitalWrite(LED1, HIGH);
	delay(Wartezeit);
	digitalWrite(LED1, LOW);

	//Warte eine bestimmte Zeit: Pause.
	delay(Wartezeit);
}
