/*****************************************************************
 * 
 * Programmiert von Ruben Vitt (2015)
 *
 * ----- Hilfen zum Verständis ---
 *
 * Anordnung der LEDs (in Klammern die Position im Array)
 *
 * .(2)   .(5)   .(8)  --- 1 Ebenen[0]
 * .(1)   .(4)   .(7)  --- 2 Ebenen[1]
 * .(0)   .(3)   .(6)  --- 3 Ebenen[2]
 *
 * 7 = Saeulen[0]
 * 8 = Saeulen[1]
 * 12 = Saeulen[2]
 * 4 = Saeulen[3]
 * 5 = Saeulen[4]
 * 6 = Saeulen[5]
 * 13 = Saeulen[6]
 * A4 = Saeulen[7]
 * A5 = Saeulen[8]
 *
 * Schalter 1 (Weiß) = Schalter[0]
 * Schalter 2 (Blau) = Schalter[1]
 * Schalter 3 (Rot)  = Schalter[2]
 *
 ***************************************************************/
 
 /******************* Inhaltsverzeichnis ***********************
 1. Konstanten, Variablen und Enumerationen
 2. loop() - Dauerschleife
 3. setup() - Startup-Methode
 3. Programmmethoden
 4. Zahlenmethoden
 5. Hilfsmethoden
 ***************************************************************/
 
//////////////////////// Konstanten /////////////////////////////
 
const char Ebenen[] = {9,10,11}; //Ebenen von unten nach unten
const char AnzahlEbenen = 3; //Anzahl der Ebenen

const char Saeulen[] = {7,8,12,4,5,6,13,A4,A5}; //Säulen nach o.a. Reihenfolge
const char AnzahlSaeulen = 9; //Anzahl der Säulen

const char Schalter[] = {A3,3,2}; //Schalter nach o.a. Reihenfolge
const char AnzahlSchalter = 3; //Anzahl der Schalter

const char Poti = A0; //Anschluss des Potentiometers

const int DauerLeuchtfeuer = 100; //Dauer der Anzeige einer LES beim Leuchtfeuer
const int Dauer = 1000; //Anzeigedauer bei den meisten Programmen
const int Zeitmultiplikator = 1; //Multiplikator für das UhrzeitProgramm (1 = Echtzeit)
const int DauerZufallsZahl = 100; //Dauer eines Bildes bei Zufallszahl

//////////////////////// Variablen /////////////////////////////

int Helligkeit = 255; //255 = ganz hell, 0 = ganz dunkel
int AktuelleZufallsZahl; //ändern bringt nichts...
int Zufall; //Zufall wird später gefüllt. Benötigt für die Zufallszahlen
int WuerfelnDauer = 13; //Dauer eines Lampenleuchten beim Würfeln

//////////////////////// Enumerationen /////////////////////////

//Auflistung der Programmtypen
//0 == Ohne Button
//1 == Button 1
//2 == Button 2
//3 == Button 3
//4 == Button 1 + 2
//5 == Button 1 + 3
//6 == Button 2 + 3
//7 == Button 1 + 2 + 3
 
const char ProgrammtypUhr = 5;
const char ProgrammtypAlleAnzeigen = 1;
const char ProgrammtypLeuchtfeuer = 4;
const char ProgrammtypZahlenfolge = 2;
const char ProgrammtypZufallsZahl = 3;
//Startprogrammtyp:
char Programm = ProgrammtypUhr;

////////////////////////////////////////////////////////////////

void loop()
{
	//Aufgabe: Schalter 1: Dimmen (alle Lampen an), Schalter 2: Matrikelnummer, Schalter 3: Lauffeuer
	
/********************************************************
*  --------------------- Verwendung ---------------------
*  --------------------- Zufallszahl --------------------
*  -------------------- Zahlenfolgen --------------------
*  Anleitung:
*  int zahlenfolge[] = {1,2,3,4,5};
*  ZahlenAnzeigen(zahlenfolge,5); -- übergeben: Array & Anzahl Elemente im Array
*  -------------------- Leuchtfeuer ---------------------
*	 Leuchtfeuer();
*  ------------------ Alle Anzeigen ---------------------
*  AlleAnzeigen();
*  ------------------ Zeit anzahlen ---------------------
*  UhrzeitAnzeigen();
********************************************************/
	if(Programm == ProgrammtypUhr)
    UhrzeitAnzeigen();
  else if(Programm == ProgrammtypAlleAnzeigen)
    AlleAnzeigen();
  else if(Programm == ProgrammtypLeuchtfeuer)
		Leuchtfeuer();
	else if(Programm == ProgrammtypZahlenfolge)
  {
		int zahlenfolge[] = {7,0,4,5,2,1,5,0};
		ZahlenAnzeigen(zahlenfolge, 8);
  }
  else if(Programm == ProgrammtypZufallsZahl)
    ZufallszahlGenerator();
  else
  {
    Delay(20);
  }
}

void setup()
{
	//Alle Saeulen als digitalen Ausgang definieren
	for(int i = 0; i < AnzahlSaeulen; i++)
		pinMode(Saeulen[i], OUTPUT);
	//Alle Ebenen als digitalen Ausgang definieren
	for(int i = 0; i < AnzahlEbenen; i++)
		pinMode(Ebenen[i], OUTPUT);
	//Schalter als Eingang definieren und Widerstand in Arduino aktivieren
	for(int i = 0; i < AnzahlSchalter; i++)
	{
		pinMode(Schalter[i], INPUT);
		digitalWrite(Schalter[i], HIGH);
	}
}










/********* Programmmethoden ***************
*
* 1. AlleAnzeigen
* 2. Leuchtfeuer
* 3. ZahlenAnzeigen
* 4. UhrzeitAnzeigen
* 5. ZufallszahlGenerator
*
*******************************************/

void AlleAnzeigen()
{
  //Jede Ebene wird in for-Schleife (einfacher und sicherer) angeschaltet
  for(int e = 0; e < AnzahlEbenen && Programm == ProgrammtypAlleAnzeigen; e++)
    an(Ebenen[e]);
  //Ebenso wird jede Säule angeschaltet
  for(int s = 0; s < AnzahlSaeulen && Programm == ProgrammtypAlleAnzeigen; s++)
    an(Saeulen[s]);
  //Für schnellen Ausstieg aus dem Programm
  Delay(5);
  aus();
}

void Leuchtfeuer()
{
  //Alle Ebenen werden durchlaufen...
  //Pro Ebene werden alle Säulen durchlaufen und einzeln angeschaltet und
  //auch wieder ausgeschaltet.
	for(int e = 0; e < AnzahlEbenen && Programm == ProgrammtypLeuchtfeuer; e++)
	{
		for(int s = 0; s < AnzahlSaeulen && Programm == ProgrammtypLeuchtfeuer; s++)
		{
			an(Ebenen[e]);
			an(Saeulen[s]);
			Delay(DauerLeuchtfeuer);
			aus(Ebenen[e]);
			aus(Saeulen[s]);
		}
	}
}

void ZahlenAnzeigen(int zahlen[], int anzahlZahlen)
{
  //Hier kann ein Array übergeben werden, welches auf dem Würfeln
  //angezeigt wird.
	if(anzahlZahlen == 0) Delay(Dauer);
	else if(anzahlZahlen == 1) EinzelneZahl(zahlen[0], 4, Dauer);
	else if(anzahlZahlen == 2) {int x = zahlen[0]; int y = zahlen[1]; int zahlen[2][2] = {{x,1},{y,2}}; ZweiZahlen(zahlen); }
	else MehrZahlen(zahlen, anzahlZahlen);
}

void UhrzeitAnzeigen()
{
  //Achtung! Anderes Zeitsystem. Ein Tag dauert effektiv 356 Sekunden länger
  //a = 41 Stunden
  //b = 46 Minuten
  //c = 46 Sekunden
  //i = Anzeigedauer
  for(int a = 0; a<41 && Programm == ProgrammtypUhr; a++)
    for(int b = 0; b < 46 && Programm == ProgrammtypUhr; b++)
      for(int c = 0; c < 46 && Programm == ProgrammtypUhr; c++)
        for(int i = 0; i < (1000 / (Zeitmultiplikator * 16)) && Programm == ProgrammtypUhr; i++)
        {
          //Sekunden oben anzeigen
          an(Ebenen[0]);
          ZeitZustand(c);
          Delay(4);
          aus();
          
          //Minuten mittig anzeigen
          an(Ebenen[1]);
          ZeitZustand(b);
          Delay(4);
          aus();
          
          //Stunden unten anzeigen
          an(Ebenen[2]);
          ZeitZustand(a);
          Delay(4);
          aus();
        }
}

void ZufallszahlGenerator()
{
  //Zufälliger Zufall
  randomSeed(Zufall);
  //Würfeln für visuelle Rückmeldung
  Wuerfeln();
  Zufallsrotation(random(1,7)); //Zufallszahl zw. 1-6 erstellen und Anzeigemethode aufrufen
}


/*********** ProgrammHilfsmethoden ***************
*
* 1. EinzelneZahl(int zahl, int ebene, int dauer)
* 2. ZweiZahlen(int zahlen[2][2]) Zahl, Ebene
* 3. MehrZahlen(int zahlen[], int anzahl)
* 4. Blinken(int anzahl, int geschwindigkeitProSekunde)
* 5. ZeitZustand(int zeit)
* 6. Wuerfeln
* 7. Zufallsrotation(int zahl)
* 8. EinsRotieren();
* 9. ZweiRotieren();
* 10. DreiRotieren();
* 11. VierRotieren();
* 12. FuenfRotieren();
* 13. SechsRotieren();
*
***************************************************/

void EinzelneZahl(int zahl, int ebene, int dauer)
{
  //soll eine einzelne Zahl angezeigt werden? - Alle Ebenen an!
	if(ebene == 4)
        {
          an(Ebenen[0]);
          an(Ebenen[1]);
          an(Ebenen[2]);
        }
        //Hier die Methode, um eine einzelne Zahlen anzuzeigen
        ebene--;
        switch(zahl)
        {
        case 0:
          an(Saeulen[6]);
          an(Ebenen[ebene]);
          Delay(dauer);
          aus(Saeulen[6]);
          aus(Ebenen[ebene]);
          break;
        case 1:
          an(Saeulen[4]);
          an(Ebenen[ebene]);
          Delay(dauer);
          aus(Saeulen[4]);
          aus(Ebenen[ebene]);
          break;
        case 2:
          an(Saeulen[2]);
          an(Saeulen[6]);
          an(Ebenen[ebene]);
          Delay(dauer);
          aus(Saeulen[2]);
          aus(Saeulen[6]);
          aus(Ebenen[ebene]);
          break;
        case 3:
          an(Saeulen[2]);
          an(Saeulen[4]);
          an(Saeulen[6]);
          an(Ebenen[ebene]);
          Delay(dauer);
          aus(Saeulen[2]);
          aus(Saeulen[4]);
          aus(Saeulen[6]);
          aus(Ebenen[ebene]);
          break;
        case 4:
          an(Saeulen[0]);
          an(Saeulen[2]);
          an(Saeulen[6]);
          an(Saeulen[8]);
          an(Ebenen[ebene]);
          Delay(dauer);
          aus(Saeulen[0]);
          aus(Saeulen[2]);
          aus(Saeulen[6]);
          aus(Saeulen[8]);
          aus(Ebenen[ebene]);
          break;
        case 5:
          an(Saeulen[0]);
          an(Saeulen[2]);
          an(Saeulen[4]);
          an(Saeulen[6]);
          an(Saeulen[8]);
          an(Ebenen[ebene]);
          Delay(dauer);
          aus(Saeulen[0]);
          aus(Saeulen[2]);
          aus(Saeulen[4]);
          aus(Saeulen[6]);
          aus(Saeulen[8]);
          aus(Ebenen[ebene]);
          break;
        case 6:
          an(Saeulen[0]);
          an(Saeulen[1]);
          an(Saeulen[2]);
          an(Saeulen[6]);
          an(Saeulen[7]);
          an(Saeulen[8]);
          an(Ebenen[ebene]);
          Delay(dauer);
          aus(Saeulen[0]);
          aus(Saeulen[1]);
          aus(Saeulen[2]);
          aus(Saeulen[6]);
          aus(Saeulen[7]);
          aus(Saeulen[8]);
          aus(Ebenen[ebene]);
          break;
        case 7:
          an(Saeulen[0]);
          an(Saeulen[1]);
          an(Saeulen[2]);
          an(Saeulen[4]);
          an(Saeulen[6]);
          an(Saeulen[7]);
          an(Saeulen[8]);
          an(Ebenen[ebene]);
          Delay(dauer);
          aus(Saeulen[0]);
          aus(Saeulen[1]);
          aus(Saeulen[2]);
          aus(Saeulen[4]);
          aus(Saeulen[6]);
          aus(Saeulen[7]);
          aus(Saeulen[8]);
          aus(Ebenen[ebene]);
          break;
        case 8:
          an(Saeulen[0]);
          an(Saeulen[1]);
          an(Saeulen[2]);
          an(Saeulen[3]);
          an(Saeulen[5]);
          an(Saeulen[6]);
          an(Saeulen[7]);
          an(Saeulen[8]);
          an(Ebenen[ebene]);
          Delay(dauer);
          aus(Saeulen[0]);
          aus(Saeulen[1]);
          aus(Saeulen[2]);
          aus(Saeulen[3]);
          aus(Saeulen[5]);
          aus(Saeulen[6]);
          aus(Saeulen[7]);
          aus(Saeulen[8]);
          aus(Ebenen[ebene]);
          break;
        case 9:
          for(int i = 0; i < AnzahlSaeulen; i++)
            an(Saeulen[i]);
          an(Ebenen[ebene]);
          Delay(dauer);
          for(int i = 0; i < AnzahlSaeulen; i++)
            aus(Saeulen[i]); 
          aus(Ebenen[ebene]);
          break;
    }
    if(ebene == 3)
      {
        aus(Ebenen[0]);
        aus(Ebenen[1]);
        aus(Ebenen[2]);
      }
}

void ZweiZahlen(int zahlen[2][2])
{
	//zahlen[0][0] an, zahlen[0][1] an, zahlen[1][0] an, zahlen[1][1] an
  //... zuruecksetzen.
  for(int i = 0; i < Dauer / 2 && Programm == ProgrammtypZahlenfolge; i++)
  {
	  EinzelneZahl(zahlen[0][0], zahlen[0][1], 1);
	  EinzelneZahl(zahlen[1][0], zahlen[1][1], 1);
  }
}

void MehrZahlen(int zahlen[], int anzahl)
{
	//in jedem Durchlauf: zahlen[i][0] an, zahlen[i][1] an.
	for(int i = 0; (i-2) <= anzahl && Programm == ProgrammtypZahlenfolge; i++)
	{
		//Wenn i = 0 ist, zeige zahlen[i] auf Ebene 1 an.
		//Wenn i = 1 ist, zeige zahlen[i-1] auf Ebene 2 an und zahlen [i] auf Ebene 1.
		//Wenn (i >= 2 && i <= anzahlZahlen) ist, zeige zahl[i-2] auf Ebene 3 an, zahlen[i-1] auf Ebene 2 und zahlen[i] auf Ebene 1.
		//Wenn ((i - 1) = anzahlZeichen) ist, zeige zahlen[i - 1] auf Ebene 3 an, zahlen[i - 2] auf Ebene 2.
	    //Wenn ((i - 2) = anzahlZeichen) ist, zeige zahlen[i - 2] auf Ebene 3 an.
    	//Sonst ist alles leer.
		if(i == 0)
			EinzelneZahl(zahlen[i], 1, Dauer);
		else if(i == 1)
		{
			int x = zahlen[i-1]; int y = zahlen[i];
			int zahlen[2][2] = {{x,2},{y,1}};
			ZweiZahlen(zahlen);
		}
		else if(i >= 2 && i < anzahl)
		{
			for(int j = 0; j < Dauer / 3 && Programm == ProgrammtypZahlenfolge; j++)
			{
				EinzelneZahl(zahlen[i - 2], 3, 1);
				EinzelneZahl(zahlen[i - 1], 2, 1);
				EinzelneZahl(zahlen[i], 1, 1);
			}
		}
		else if((i) == anzahl)
		{
			int x = zahlen[anzahl - 1]; int y = zahlen[anzahl - 2];
			int zahlen[2][2] = {{x, 2}, {y, 3}};
			ZweiZahlen(zahlen);
		}
		else if((i - 1) == anzahl)
		{
			EinzelneZahl(zahlen[anzahl - 1], 3, Dauer);
		}
		else {Delay(Dauer / 3); }
	}
	Blinken(3,4);
	Delay(Dauer / 3);
}

void Blinken(int anzahl, int geschwindigkeitProSekunde)
{
	//Alle LEDs blinken x mal für y-Sekundelstel
	for(int a = 0; a < anzahl && Programm == ProgrammtypZahlenfolge; a++)
	{
		for(int i = 0; i < AnzahlSaeulen; i++)
			an(Saeulen[i]);
		for(int i = 0; i < AnzahlEbenen; i++)
			an(Ebenen[i]);
		Delay((1000 / geschwindigkeitProSekunde) / 2);
		aus();
		Delay((1000 / geschwindigkeitProSekunde) / 2);
	}
}

void ZeitZustand(int zeit)
{
  //In dieser Methoden werden alle mölichen Zahlzustände definiert
  switch(zeit) 
  {
    case 1:
    an(Saeulen[0]);
    break;
    case 2:
    an(Saeulen[1]);
    break;
    case 3:
    an(Saeulen[2]);
    break;
    case 4:
    an(Saeulen[3]);
    break;
    case 5:
    an(Saeulen[4]);
    break;
    case 6:
    an(Saeulen[5]);
    break;
    case 7:
    an(Saeulen[6]);
    break;
    case 8:
    an(Saeulen[7]);
    break;
    case 9:
    an(Saeulen[8]);
    break;
    case 10:
    an(Saeulen[0]); an(Saeulen[1]);
    break;
    case 11:
    an(Saeulen[0]); an(Saeulen[2]);
    break;
    case 12:
    an(Saeulen[0]); an(Saeulen[3]);
    break;
    case 13:
    an(Saeulen[0]); an(Saeulen[4]);
    break;
    case 14:
    an(Saeulen[0]); an(Saeulen[5]);
    break;
    case 15:
    an(Saeulen[0]); an(Saeulen[6]);
    break;
    case 16:
    an(Saeulen[0]); an(Saeulen[7]);
    break;
    case 17:
    an(Saeulen[0]); an(Saeulen[8]);
    break;
    case 18:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]);
    break;
    case 19:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[3]);
    break;
    case 20:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[4]);
    break;
    case 21:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[5]);
    break;
    case 22:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[6]);
    break;
    case 23:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[7]);
    break;
    case 24:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[8]);
    break;
    case 25:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[3]);
    break;
    case 26:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[4]);
    break;
    case 27:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[5]);
    break;
    case 28:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[6]);
    break;
    case 29:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[7]);
    break;
    case 30:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[8]);
    break;
    case 31:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[3]); an(Saeulen[4]);
    break;
    case 32:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[3]); an(Saeulen[5]);
    break;
    case 33:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[3]); an(Saeulen[6]);
    break;
    case 34:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[3]); an(Saeulen[7]);
    break;
    case 35:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[3]); an(Saeulen[8]);
    break;
    case 36:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[3]); an(Saeulen[4]); an(Saeulen[5]);
    break;
    case 37:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[3]); an(Saeulen[4]); an(Saeulen[6]);
    break;
    case 38:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[3]); an(Saeulen[4]); an(Saeulen[7]);
    break;
    case 39:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[3]); an(Saeulen[4]); an(Saeulen[8]);
    break;
    case 40:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[3]); an(Saeulen[4]); an(Saeulen[5]); an(Saeulen[6]);
    break;
    case 41:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[3]); an(Saeulen[4]); an(Saeulen[5]); an(Saeulen[7]);
    break;
    case 42:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[3]); an(Saeulen[4]); an(Saeulen[5]); an(Saeulen[8]);
    break;
    case 43:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[3]); an(Saeulen[4]); an(Saeulen[5]); an(Saeulen[6]); an(Saeulen[7]);
    break;
    case 44:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[3]); an(Saeulen[4]); an(Saeulen[5]); an(Saeulen[6]); an(Saeulen[8]);
    break;
    case 45:
    an(Saeulen[0]); an(Saeulen[1]); an(Saeulen[2]); an(Saeulen[3]); an(Saeulen[4]); an(Saeulen[5]); an(Saeulen[6]); an(Saeulen[7]); an(Saeulen[8]);
    break;
  }
}
  
  void Wuerfeln()
  {
    //LEDs zeigen ein schnelles => LEUCHTFEUER <= an
    	for(int e = 0; e < AnzahlEbenen && Programm == ProgrammtypZufallsZahl; e++)
	     {
		      for(int s = 0; s < AnzahlSaeulen && Programm == ProgrammtypZufallsZahl; s++)
      		{
	       		an(Ebenen[e]);
      			an(Saeulen[s]);
		      	Delay(WuerfelnDauer);
      			aus(Ebenen[e]);
		      	aus(Saeulen[s]);
	       	}
     	}
  }
  
  
/* Anordnung der LEDs (in Klammern die Position im Array)
 *
 * .(2)   .(5)   .(8)  --- 1 Ebenen[0]
 * .(1)   .(4)   .(7)  --- 2 Ebenen[1]
 * .(0)   .(3)   .(6)  --- 3 Ebenen[2]
 *
 * 7 = Saeulen[0]
 * 8 = Saeulen[1]
 * 12 = Saeulen[2]
 * 4 = Saeulen[3]
 * 5 = Saeulen[4]
 * 6 = Saeulen[5]
 * 13 = Saeulen[6]
 * A4 = Saeulen[7]
 * A5 = Saeulen[8] */
void Zufallsrotation(int nummer)
{
  //Auf Zufallszahl prüfen und dann Methode zum Rotieren aufrufen
  AktuelleZufallsZahl = nummer;
  switch(nummer)
  {
    case 1:
      EinsRotieren();
    break;
    case 2:
      ZweiRotieren();
    break;
    case 3:
      DreiRotieren();
    break;
    case 4:
      VierRotieren();
    break;
    case 5:
      FuenfRotieren();
    break;
    case 6:
      SechsRotieren();
    break;
  }
}

void EinsRotieren()
{
  while(AktuelleZufallsZahl == 1 && Programm == ProgrammtypZufallsZahl)
  {
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
  }
}

void ZweiRotieren()
{
  while(AktuelleZufallsZahl == 2 && Programm == ProgrammtypZufallsZahl)
  {
    //8 mögliche Rotationen
    //Rotationsbild 1
    for(int i = 0; i <= DauerZufallsZahl / 2 && AktuelleZufallsZahl == 2 && Programm == ProgrammtypZufallsZahl; i++)
    {
      an(Saeulen[8]);
      an(Ebenen[0]);
      Delay(1);
      aus(Saeulen[8]);
      aus(Ebenen[0]);
      if(Programm != ProgrammtypZufallsZahl) break;
      an(Saeulen[0]);
      an(Ebenen[2]);
      Delay(1);
      aus(Saeulen[0]);
      aus(Ebenen[2]);
    }
    //Rotationsbild 2
    for(int i = 0; i <= DauerZufallsZahl / 2 && AktuelleZufallsZahl == 2 && Programm == ProgrammtypZufallsZahl; i++)
    {
      an(Saeulen[7]);
      an(Ebenen[0]);
      Delay(1);
      aus(Saeulen[7]);
      aus(Ebenen[0]);
      if(Programm != ProgrammtypZufallsZahl) break;
      an(Saeulen[1]);
      an(Ebenen[2]);
      Delay(1);
      aus(Saeulen[1]);
      aus(Ebenen[2]);
    }
    //Rotationsbild 3
    for(int i = 0; i <= DauerZufallsZahl / 2 && AktuelleZufallsZahl == 2 && Programm == ProgrammtypZufallsZahl; i++)
    {
      an(Saeulen[6]);
      an(Ebenen[0]);
      Delay(1);
      aus(Saeulen[6]);
      aus(Ebenen[0]);
      if(Programm != ProgrammtypZufallsZahl) break;
      an(Saeulen[2]);
      an(Ebenen[2]);
      Delay(1);
      aus(Saeulen[2]);
      aus(Ebenen[2]);
    }
    //Rotationsbild 4
    for(int i = 0; i <= DauerZufallsZahl / 2 && AktuelleZufallsZahl == 2 && Programm == ProgrammtypZufallsZahl; i++)
    {
      an(Saeulen[3]);
      an(Ebenen[0]);
      Delay(1);
      aus(Saeulen[3]);
      aus(Ebenen[0]);
      if(Programm != ProgrammtypZufallsZahl) break;
      an(Saeulen[5]);
      an(Ebenen[2]);
      Delay(1);
      aus(Saeulen[5]);
      aus(Ebenen[2]);
    }
    //Rotationsbild 5
    for(int i = 0; i <= DauerZufallsZahl / 2 && AktuelleZufallsZahl == 2 && Programm == ProgrammtypZufallsZahl; i++)
    {
      an(Saeulen[0]);
      an(Ebenen[0]);
      Delay(1);
      aus(Saeulen[0]);
      aus(Ebenen[0]);
      if(Programm != ProgrammtypZufallsZahl) break;
      an(Saeulen[8]);
      an(Ebenen[2]);
      Delay(1);
      aus(Saeulen[8]);
      aus(Ebenen[2]);
    }
    //Rotationsbild 6
    for(int i = 0; i <= DauerZufallsZahl / 2 && AktuelleZufallsZahl == 2 && Programm == ProgrammtypZufallsZahl; i++)
    {
      an(Saeulen[1]);
      an(Ebenen[0]);
      Delay(1);
      aus(Saeulen[1]);
      aus(Ebenen[0]);
      if(Programm != ProgrammtypZufallsZahl) break;
      an(Saeulen[7]);
      an(Ebenen[2]);
      Delay(1);
      aus(Saeulen[7]);
      aus(Ebenen[2]);
    }
      //Rotationsbild 7
    for(int i = 0; i <= DauerZufallsZahl / 2 && AktuelleZufallsZahl == 2 && Programm == ProgrammtypZufallsZahl; i++)
    {
      an(Saeulen[2]);
      an(Ebenen[0]);
      Delay(1);
      aus(Saeulen[2]);
      aus(Ebenen[0]);
      if(Programm != ProgrammtypZufallsZahl) break;
      an(Saeulen[6]);
      an(Ebenen[2]);
      Delay(1);
      aus(Saeulen[6]);
      aus(Ebenen[2]);
    }
    //Rotationsbild 8
    for(int i = 0; i <= DauerZufallsZahl / 2 && AktuelleZufallsZahl == 2 && Programm == ProgrammtypZufallsZahl; i++)
    {
      an(Saeulen[5]);
      an(Ebenen[0]);
      Delay(1);
      aus(Saeulen[5]);
      aus(Ebenen[0]);
      if(Programm != ProgrammtypZufallsZahl) break;
      an(Saeulen[3]);
      an(Ebenen[2]);
      Delay(1);
      aus(Saeulen[3]);
      aus(Ebenen[2]);
    }
  }
}

void DreiRotieren()
{
  while(AktuelleZufallsZahl == 3 && Programm == ProgrammtypZufallsZahl)
 {
  //8 mögliche Rotationen
  //Rotationsbild 1
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 3 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[8]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[8]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[0]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[0]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 2
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 3 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[7]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[7]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[1]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[1]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 3
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 3 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[6]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[6]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[2]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[2]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 4
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 3 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[3]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[3]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[5]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[5]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 5
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 3 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[0]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[0]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[8]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[8]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 6
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 3 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[1]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[1]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[7]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[7]);
    aus(Ebenen[2]);
  }
    //Rotationsbild 7
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 3 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[2]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[2]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[6]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[6]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 8
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 3 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[5]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[5]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[3]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[3]);
    aus(Ebenen[2]);
  }
 }
}

void VierRotieren()
{
 while(AktuelleZufallsZahl == 4 && Programm == ProgrammtypZufallsZahl)
 {
  //8 mögliche Rotationen
  //Rotationsbild 1
  for(int i = 0; i <= DauerZufallsZahl / 2 && AktuelleZufallsZahl == 4 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[8]);
    an(Saeulen[6]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[8]);
    aus(Saeulen[6]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[0]);
    an(Saeulen[2]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[0]);
    aus(Saeulen[2]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 2
  for(int i = 0; i <= DauerZufallsZahl / 2 && AktuelleZufallsZahl == 4 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[7]);
    an(Saeulen[3]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[7]);
    aus(Saeulen[3]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[5]);
    an(Saeulen[1]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[5]);
    aus(Saeulen[1]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 3
  for(int i = 0; i <= DauerZufallsZahl / 2 && AktuelleZufallsZahl == 4 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[6]);
    an(Saeulen[0]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[6]);
    aus(Saeulen[0]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[8]);
    an(Saeulen[2]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[8]);
    aus(Saeulen[2]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 4
  for(int i = 0; i <= DauerZufallsZahl / 2 && AktuelleZufallsZahl == 4 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[3]);
    an(Saeulen[1]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[3]);
    aus(Saeulen[1]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[7]);
    an(Saeulen[5]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[7]);
    aus(Saeulen[5]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 5
  for(int i = 0; i <= DauerZufallsZahl / 2 && AktuelleZufallsZahl == 4 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[0]);
    an(Saeulen[2]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[0]);
    aus(Saeulen[2]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[8]);
    an(Saeulen[6]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[8]);
    aus(Saeulen[6]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 6
  for(int i = 0; i <= DauerZufallsZahl / 2 && AktuelleZufallsZahl == 4 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[1]);
    an(Saeulen[5]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[1]);
    aus(Saeulen[5]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[7]);
    an(Saeulen[3]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[3]);
    aus(Saeulen[7]);
    aus(Ebenen[2]);
  }
    //Rotationsbild 7
  for(int i = 0; i <= DauerZufallsZahl / 2 && AktuelleZufallsZahl == 4 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[2]);
    an(Saeulen[8]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[8]);
    aus(Saeulen[2]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[0]);
    an(Saeulen[6]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[0]);
    aus(Saeulen[6]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 8
  for(int i = 0; i <= DauerZufallsZahl / 2 && AktuelleZufallsZahl == 4 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[5]);
    an(Saeulen[7]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[5]);
    aus(Saeulen[7]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[1]);
    an(Saeulen[3]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[1]);
    aus(Saeulen[3]);
    aus(Ebenen[2]);
  }
 }
}

void FuenfRotieren()
{
 while(AktuelleZufallsZahl == 5 && Programm == ProgrammtypZufallsZahl)
 {
  //8 mögliche Rotationen
  //Rotationsbild 1
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 5 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[8]);
    an(Saeulen[6]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[8]);
    aus(Saeulen[6]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[0]);
    an(Saeulen[2]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[0]);
    aus(Saeulen[2]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 2
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 5 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[7]);
    an(Saeulen[3]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[7]);
    aus(Saeulen[3]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[5]);
    an(Saeulen[1]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[5]);
    aus(Saeulen[1]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 3
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 5 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[6]);
    an(Saeulen[0]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[6]);
    aus(Saeulen[0]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[8]);
    an(Saeulen[2]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[8]);
    aus(Saeulen[2]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 4
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 5 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[3]);
    an(Saeulen[1]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[3]);
    aus(Saeulen[1]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[7]);
    an(Saeulen[5]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[7]);
    aus(Saeulen[5]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 5
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 5 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[0]);
    an(Saeulen[2]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[0]);
    aus(Saeulen[2]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[8]);
    an(Saeulen[6]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[8]);
    aus(Saeulen[6]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 6
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 5 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[1]);
    an(Saeulen[5]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[1]);
    aus(Saeulen[5]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[7]);
    an(Saeulen[3]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[3]);
    aus(Saeulen[7]);
    aus(Ebenen[2]);
  }
    //Rotationsbild 7
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 5 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[2]);
    an(Saeulen[8]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[8]);
    aus(Saeulen[2]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[0]);
    an(Saeulen[6]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[0]);
    aus(Saeulen[6]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 8
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 5 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[5]);
    an(Saeulen[7]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[5]);
    aus(Saeulen[7]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[4]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[4]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[1]);
    an(Saeulen[3]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[1]);
    aus(Saeulen[3]);
    aus(Ebenen[2]);
  }
 }
}

void SechsRotieren()
{
  while(AktuelleZufallsZahl == 6 && Programm == ProgrammtypZufallsZahl)
 {
  //8 mögliche Rotationen
  //Rotationsbild 1
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 6 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[8]);
    an(Saeulen[6]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[8]);
    aus(Saeulen[6]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[5]);
    an(Saeulen[3]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[5]);
    aus(Saeulen[3]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[0]);
    an(Saeulen[2]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[0]);
    aus(Saeulen[2]); 
    aus(Ebenen[2]);
  }
  //Rotationsbild 2
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 6 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[3]);
    an(Saeulen[7]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[3]);
    aus(Saeulen[7]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[0]);
    an(Saeulen[8]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[0]);
    aus(Saeulen[8]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[5]);
    an(Saeulen[1]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[5]);
    aus(Saeulen[1]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 3
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 6 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[6]);
    an(Saeulen[0]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[0]);
    aus(Saeulen[6]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[7]);
    an(Saeulen[1]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[7]);
    aus(Saeulen[1]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[8]);
    an(Saeulen[2]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[8]);
    aus(Saeulen[2]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 4
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 6 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[3]);
    an(Saeulen[1]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[3]);
    aus(Saeulen[1]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[6]);
    an(Saeulen[2]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[6]);
    aus(Saeulen[2]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[7]);
    an(Saeulen[5]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[7]);
    aus(Saeulen[5]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 5
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 6 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[0]);
    an(Saeulen[2]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[0]);
    aus(Saeulen[2]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[5]);
    an(Saeulen[3]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[5]);
    aus(Saeulen[3]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[8]);
    an(Saeulen[6]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[8]);
    aus(Saeulen[6]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 6
  for(int i = 0; i <= DauerZufallsZahl /3 && AktuelleZufallsZahl == 6 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[1]);
    an(Saeulen[5]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[1]);
    aus(Saeulen[5]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[8]);
    an(Saeulen[0]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[8]);
    aus(Saeulen[0]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[7]);
    an(Saeulen[3]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[7]);
    aus(Saeulen[3]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 7
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 6 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[8]);
    an(Saeulen[2]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[8]);
    aus(Saeulen[2]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[7]);
    an(Saeulen[1]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[7]);
    aus(Saeulen[1]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[0]);
    an(Saeulen[6]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[0]);
    aus(Saeulen[6]);
    aus(Ebenen[2]);
  }
  //Rotationsbild 8
  for(int i = 0; i <= DauerZufallsZahl / 3 && AktuelleZufallsZahl == 6 && Programm == ProgrammtypZufallsZahl; i++)
  {
    an(Saeulen[5]);
    an(Saeulen[7]);
    an(Ebenen[0]);
    Delay(1);
    aus(Saeulen[5]);
    aus(Saeulen[7]);
    aus(Ebenen[0]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[2]);
    an(Saeulen[6]);
    an(Ebenen[1]);
    Delay(1);
    aus(Saeulen[2]);
    aus(Saeulen[6]);
    aus(Ebenen[1]);
    if(Programm != ProgrammtypZufallsZahl) break;
    an(Saeulen[1]);
    an(Saeulen[3]);
    an(Ebenen[2]);
    Delay(1);
    aus(Saeulen[1]);
    aus(Saeulen[3]);
    aus(Ebenen[2]);
  }
 }
}

/************ Hilfsmethoden ****************
*
* 1. an(int port) schaltet angegeben Pin an
* 2. aus(int port) schaltet angegebenen Pin aus
* 3. aus() schaltet alles aus
* 3. Delay(int dauer) angepasste delay()-Methode
* 4. SchalterGedrueckt(char nummer)
*
*******************************************/

void an(int port)
{
	//Welcher Port? Bei Ebenen analog die Helligkeit schreiben...
	switch(port)
	{
		case 9:
		case 10:
		case 11:
		  analogWrite(port, Helligkeit);
		  break;
		default:
		  digitalWrite(port, HIGH);
		  break;
	}
}

void aus(int port)
{
	//Den übergebenen Port ausschalten
	digitalWrite(port, LOW);
}

void aus()
{
	//Alle Pins ausschalten
	for(int i = 0; i < AnzahlSaeulen; i++)
		aus(Saeulen[i]);
	for(int i = 0; i < AnzahlEbenen; i++)
		aus(Ebenen[i]);
}

void Delay(int dauer) 
{
	//Benutzerdefinierte Delay-Methode überprüft das Poti und die Buttons
	//#Echtzeit-Eingriff
	int j = 0;
	for(int i = 0; i <= dauer; i++)
	{
		//Poti prüfen
		Helligkeit = (analogRead(Poti) / 4);
    // //Drei Buttons(1, 2 und 3) prüfen
    // if(digitalRead(Schalter[0]) == LOW && digitalRead(Schalter[1]) == LOW && digitalRead(Schalter[2]) == LOW)
    // {
    //   delay(200);
    //   SchalterGedrueckt(6);
    // }
    
    
    //Zwei Buttons(1 und 2) prüfen
    if(digitalRead(Schalter[0]) == LOW && digitalRead(Schalter[1]) == LOW)
    {
      delay(200);
      SchalterGedrueckt(3);
    }
     //Zwei Buttons(1 und 3) prüfen
    else if(digitalRead(Schalter[0]) == LOW && digitalRead(Schalter[2]) == LOW)
    {
      delay(200);
      SchalterGedrueckt(4);
    }
     //Zwei Buttons(2 und 3) prüfen
    // else if(digitalRead(Schalter[1]) == LOW && digitalRead(Schalter[2]) == LOW)
    // {
    //   delay(200);
    //   SchalterGedrueckt(5);
    // }
    
		//Button 1 prüfen
		else if(digitalRead(Schalter[0]) == LOW)
			SchalterGedrueckt(0);
		//Button 2 prüfen
		else if(digitalRead(Schalter[1]) == LOW)
			SchalterGedrueckt(1);
		//Button 3 prüfen
		else if(digitalRead(Schalter[2]) == LOW)
			SchalterGedrueckt(2);
		delay(1);
    Zufall++;
	}
  //Zufallsgenerator hochzählen
}

void SchalterGedrueckt(char nummer)
{
  AktuelleZufallsZahl = random(1,6);
	switch(nummer)
	{
		case 0:
			Programm = 1;
			break;
		case 1:
			Programm = 2;
			break;
		case 2:
			Programm = 3;
			break;
    case 3:
      Programm = 4;
      break;
    case 4:
      Programm = 5;
      break;
    case 5:
      Programm = 6;
      break;
    case 6:
      Programm = 7;
      break;
	}
}
}