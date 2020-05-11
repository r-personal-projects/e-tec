#include <EnableInterrupt.h>
/***************************************************
 * Programmiert von Ruben Vitt (2015)
 * 
 * ------ Hilfen zum Verständnis -------
 * 
 * Anordnung der LEDs (in Klammern die Position im Array)
 * 
 * .(2)   .(5)   .(8)  --- 1
 * .(1)   .(4)   .(7)  --- 2
 * .(0)   .(3)   .(6)  --- 3
 *
 * Schalter 1 = Weiß = Schalter[0]
 * Schalter 2 = Blau = Schalter[1]
 * Schalter 3 = Rot = Schalter[2]
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
 ****************************************************/
//////////////////////////////////////////////////////////////
static int Ebenen[] = {9,10,11}; //Ebenen von oben nach unten
static int AnzahlEbenen = 3; //Anzahl der Ebenen
static int Saeulen[] = {7,8,12,4,5,6,13,A4,A5}; //Säulen nach Reihenfolge
static int AnzahlSaeulen = 9; //Anzahl der Säulen
static int Schalter[] = {A3,3,2}; //Schalter nach Reihenfolge (w-b-r, 1-2-3)
static int AnzahlSchalter = 3; //Anzahl der Schalter
static int ProgrammUhrAnzeigen = 0; //Programmnummer der Pseudouhr
static int ProgrammAlleAnzeigen = 1; //Programmnummer des Programms "Alle Anzeigen"
static int ProgrammLeuchtfeuer = 2; //Programmnummer des Leuchtfeuers
static int ProgrammMatrikelnummer = 3; //Programmnummer der Matrikelnummer
static int Poti = A0; //Anschluss des Potis

static int LeuchtfeuerDauer = 200; //Wie schnell soll die LED wechseln beim Leuchtfeuer?
int Helligkeit = 255; //Helligkeit für PWM
static int Dauer = 800; //Globale Anzeigedauer bei den meisten Programmen
int ZeitMultiplikator = 1; //Multiplikator für das ProgrammUhrAnzeigen (1 = Echtzeit)
int Programm = 0; //Welches Programm soll standardmäßig ausgeführt werden?
//////////////////////////////////////////////////////////////

void loop()
{
/********************************************************
*  --------------------- Verwendung ---------------------
*  Egal wieviele Zahlen, die Methode nimmt IMMER ein Array an 
*  und außerdem die Anzahl in int der Elemente darin.
*  Beispiele:
*  ZahlenAnzeigen({0}, 1) zeigt eine 0 auf dem Cube an
*  ZahlenAnzeigen({0, 5}, 2) zeigt eine 0 und eine fünf auf dem Cube an
*  ZahlenAnzeigen({0,1,2,3,4,5,6,7,8,9}, 10) zeigt die Zahlen hintereinander an.
*  --------------------- Zufallszahl --------------------
*  Anleitung:
*  int x = rand();
*  int zahlen[] = {x,x,x};
*  ZahlenAnzeigen(zahlen,3);
*  ------------------- Matrikelnummer -------------------
*  Anleitung:
*  int matrikelnummer[] = {1,2,3,4,5};
*  ZahlenAnzeigen(matrikelnummer,5);
*  -------------------- Leuchtfeuer ---------------------
*  Anleitung:
*  Leuchtfeuer();
*  ------------------ Alle Anzeigen ---------------------
*  AlleAnzeigen();
*  ------------------ Zeit anzahlen ---------------------
*  ZeitZaehlen();
********************************************************/
  if(Programm == ProgrammUhrAnzeigen)
    ZeitZaehlen();
  if(Programm == ProgrammAlleAnzeigen)
    AlleAnzeigen();
  if(Programm == ProgrammLeuchtfeuer)
    Leuchtfeuer();
  if(Programm == 3)
  {
    int matrikelnummer[] = {1,2,3,4,5};
    ZahlenAnzeigen(matrikelnummer,5);
  }
}














void setup()
{
  Serial.begin(9600);
  for(int i = 0; i < AnzahlSaeulen; i++)
    pinMode(Saeulen[i], OUTPUT);
  //enableInterrupt(Schalter[0], Schalter1Gedrueckt, LOW);
  //enableInterrupt(Schalter[1], Schalter2Gedrueckt, LOW);
  //enableInterrupt(Schalter[2], Schalter3Gedrueckt, LOW);
  for(int i = 0; i < AnzahlSchalter; i++)
  {
    pinMode(Schalter[i], INPUT);   
    digitalWrite(Schalter[i], HIGH);
  }
}

/*********************************************************
*                  Interrupts                            *
* Grundzustand: Uhr
* Schalter 1: Dimmen (alle Lampen an)
* Schalter 2: Matrikelnummer
* Schalter 3: Lauffeuer
*********************************************************/
void AlleAnzeigen()
{
  while(Programm == ProgrammAlleAnzeigen)
  {
    int zahlen[] = {9};
    ZahlenAnzeigen(zahlen, 1);
  }
}

void ZeitZaehlen() 
{ 
    for(int a = 0; a<41 && Programm == ProgrammUhrAnzeigen;a++)
    {
      for(int b = 0; b<46 && Programm == ProgrammUhrAnzeigen;b++)
      {
        for(int c = 0; c<46 && Programm == ProgrammUhrAnzeigen;c++)
        {
          for(int i = 0; i < (1000 / (ZeitMultiplikator * 16)) && Programm == ProgrammUhrAnzeigen;i++)
          {
            an(Ebenen[0]);
            Zustand(c);
            Delay(4);
            aus(Ebenen[0]);
            aus(); //Alle Säulen ausschalten
            
            an(Ebenen[1]);
            Zustand(b);
            Delay(4);
            aus(Ebenen[1]);
            aus();
            
            an(Ebenen[2]);
            Zustand(a);
            Delay(4);
            aus(Ebenen[2]);
            aus();
          }
        }
      }
    }
}

void Zustand(int nummer)
{
  switch(nummer) 
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


void ZahlenAnzeigen(int zahlen[], int anzahlZahlen) 
{
  //Wenn es eine Zahl ist, mache...
  if(anzahlZahlen == 0) Delay(Dauer);
  else if(anzahlZahlen == 1) EinzelneZahl(zahlen[0], 4, Dauer);
  else if(anzahlZahlen == 2) {int x = zahlen[0]; int y = zahlen[1]; int zahlen[2][2] = {{x, 1}, {y, 2}}; ZweiZahlen(zahlen); }
  else MehrZahlen(zahlen, anzahlZahlen);
}

void ZweiZahlen(int zahlen[2][2])
{
  //zahlen[0][0] an, zahlen[0][1] an, zahlen[1][0] an, zahlen[1][1] an
  //... zuruecksetzen.
  for(int i = 0; i < Dauer / 2; i++)
  {
    EinzelneZahl(zahlen[0][0], zahlen[0][1], 1);
    EinzelneZahl(zahlen[1][0], zahlen[1][1], 1);
  }
}

void MehrZahlen(int zahlen[], int anzahl)
{
  //in jedem Durchlauf: zahlen[i][0] an, zahlen[i][1] an.
  for(int i = 0; (i-2) <= anzahl; i++)
  {
    //Wenn i = 0 ist, zeige zahlen[i] auf Ebene 1 an.
    //Wenn i = 1 ist, zeige zahlen[i-1] auf Ebene 2 an und zahlen[i] auf Ebene 1.
    //Wenn (i >= 2 && i <= anzahlZahlen) ist, zeige zahlen[i-2] auf Ebene 3 an, zahlen[i-1] auf Ebene 2 und zahlen[i] auf Ebene 1.
    //Wenn ((i - 1) = anzahlZeichen) ist, zeige zahlen[i - 1] auf Ebene 3 an, zahlen[i - 2] auf Ebene 2.
    //Wenn ((i - 2) = anzahlZeichen) ist, zeige zahlen[i - 2] auf Ebene 3 an.
    //Sonst ist alles leer.
      if(i == 0)
        EinzelneZahl(zahlen[i], 1, Dauer);
      else if(i == 1)
      {
        int x = zahlen [i-1]; int y = zahlen[i];
        int zahlen[2][2] = {{x, 2}, {y, 1}};
        ZweiZahlen(zahlen);
      }
      else if(i >= 2 && i < anzahl) 
      {
        for(int j = 0; j < Dauer / 3; j++)
        {
        //Ebene an
        //Zahl an
        //Delay 2
        //beides aus
        
        //Ebene an
        //Zahl an
        //Delay 2
        //beides aus
        
        //Ebene an
        //Zahl an
        //Delay 2
        //beides aus
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
      else if((i-1) == anzahl)
      {
        EinzelneZahl(zahlen[anzahl - 1], 3, Dauer);
      }
      else { Delay(Dauer / 3); }
    }
    Blinken(3,4);
    Delay(Dauer / 3);
}

void Blinken(int anzahl, int geschwindigkeitProSekunde)
{
  //Alle LEDs blinken x mal für y-Sekundelstel
  for(int a = 0; a < anzahl; a++)
  {
    for(int i = 0; i < AnzahlSaeulen; i++)
      an(Saeulen[i]);
    for(int i = 0; i < AnzahlEbenen; i++)
      an(Ebenen[i]);
    Delay(((1000 / geschwindigkeitProSekunde) / 2));
    for(int i = 0; i < AnzahlSaeulen; i++)
      aus(Saeulen[i]);
    for(int i = 0; i < AnzahlEbenen; i++)
      aus(Ebenen[i]);
    Delay(((1000 / geschwindigkeitProSekunde) / 2));
  }
  
}

void Delay(int dauer)
{ 
  int j = 0;
  for(int i = 0; i <= dauer; i++)
  {
     Helligkeit = (analogRead(Poti) / 4);
     delay(1);
     if(digitalRead(Schalter[0]) == LOW)
       Schalter1Gedrueckt();
     if(digitalRead(Schalter[1]) == LOW)
       Schalter2Gedrueckt();
//     if(digitalRead(Schalter[2]) == LOW)
//       Schalter3Gedrueckt();
  } 
}


void Schalter1Gedrueckt() 
{
  Programm = ProgrammAlleAnzeigen;
}

void Schalter2Gedrueckt()
{
  Programm = ProgrammLeuchtfeuer;
}

void Schalter3Gedrueckt()
{
  Programm = ProgrammMatrikelnummer;
}

/*void SchalterGedrueckt(int nummer)
{
  Programm = nummer;
  switch(nummer)
  {
    case 1:
      Serial.println("Schalter 1");
      break;
    case 2:
      Serial.println("Schalter 2");
      break;
    case 3:
      Serial.println("Schalter 3");
      break;
    default:
      Serial.println("Unbekannt");
      break;
  }
}*/

void an(int port)
{
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
  digitalWrite(port, LOW);
}

void aus() 
{
  for(int i = 0; i<AnzahlSaeulen;i++)
  {
    aus(Saeulen[i]); 
  }
  for(int i = 0; i<AnzahlEbenen;i++)
  {
    aus(Ebenen[i]);
  }
}