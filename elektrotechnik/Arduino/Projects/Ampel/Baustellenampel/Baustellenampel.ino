#include <Wire.h>
//PIN-Belegungen
  //Ausgänge Rubeen
  /*const int autoRot = 6;
  const int autoGelb = 5;
  const int autoGruen = 4;
  const int fussRot = 3;
  const int fussGruen = 2;*/
  const int autoRot = 7;
  const int autoGelb = 6;
  const int autoGruen = 5;
  const int fussRot = 4;
  const int fussGruen = 3;
  
  //Ausgänge Dominik
  /*const int autoRot = 4;
  const int autoGelb = 3;
  const int autoGruen = 2;
  const int fussRot = 6;
  const int fussGruen = 5;*/
  
  const int pinImmerAn = A0;
  //Eingänge
  const int dip1 = 7;
  const int dip2 = 8;
  const int dip3 = 9;
  const int dip4 = 10;
  const int bedarfsTaster = 11;
//Programmarten
  const int artBlinken = 0;
  const int artAus = 1;
  const int artBaustelle = 2;
  //Aktuelle Programmart speichern: (Auf Anfangswert setzen)
  int programmArt = artBlinken;
//Laufzeitvariablen
  //master statt 0 benutzen: übersichtlicher
  const int master = 0;
  int adresse = master;
  //Standardblinkdauer setzen, kann durch Befehl geändert werden
  int blinkdauer = 1000;
  String anweisung = "";

void setup()
{
  //Inputs & Outputs definieren
  PinModesSetzen();
  //Aktuelle Adresse von DIP-Schalter lesen
  adresse = AdresseLesen();
  //Konsolenkommunikation beginnen
  Serial.begin(9600);
  Wire.begin();
  //2. Stromquelle anschalten
  an(pinImmerAn);
}

void loop()
{
  //Wenn MASTER, dann führe das MASTER-Programm aus...
  if(adresse == master)
    {
      Master();
      Programm();
    }
  //...sonst höre auf den MASTER
  else
  {
    Anweisungen();
  }
}

void Programm()
{
  //Aktuelles Programm wählen... (Nur MASTER)
  switch(programmArt)
  {
    //Ampeln werden ausgeschaltet
    case artAus:
      Serial.println("Die Ampeln werden ausgeschaltet.");
      Aus();
      break;
    //Baustellenampel wird gestartet
    case artBaustelle:
      Serial.println("Die Ampel wurde auf den Baustellenmodus gestellt.");
      BaustellenAmpel();
      break;
    //Standard: Blinken (zur Sicherheit)
    default:
      Serial.println("Die Ampeln schalten in den Sicherheitsmodus.");
      Blinken();
      break;
  }
}

/****** Programme ******
* 1. Blinken()
* 2. Aus()
* 3. Periodendauer(int)
* 4. Baustellenampel()
* 4. SetzeRot()
* 5. SetzeGelbRot()
* 6. SetzeGruen()
* 7. SetzeGelb()
***********************/
void Blinken()
{
  //MASTER-Programm
  if(adresse == master)
  {
    //An die Ampel 1 wird der AUS-Befehl gesendet (da kann ja momentan etwas laufen...)
    SendeText("AUS", 1);
    //Während die richtige Programmart eingestellt ist und MASTER da ist...
    while(programmArt == artBlinken && adresse == master)
    {
      //Sende den GELB-Befehl an A1 
      SendeText("SETZE GELB", 1);
      //Synchronität: Setze diese Ampel ebenfalls auf gelb
      SetzeGelb();
      //Warte solange, wie die Blinkdauer eingestellt ist
      Delay(blinkdauer);
      //Schalte dann beide Ampeln aus
      SendeText("AUS", 1);
      aus(autoGelb);
      //Und warte wieder...
      Delay(blinkdauer);
    }
  }
}

void Aus()
{
  while(programmArt == artAus && adresse == master)
  {
    //Alle werden ausgeschaltet, zur Sicherheit wird 1 Sek gewartet
    alleAus();
    Delay(1000);
  }
}

void Periodendauer(int x)
{
  //Blinkdauer wird gesetzt
  blinkdauer = x * 1000;
  Serial.print("Die neue Periodendauer betraegt nun ");
  Serial.print(blinkdauer / 1000);
  Serial.println(" Sekunde(n).");
}

void BaustellenAmpel()
{
  //MASTER-PROGRAMM
  while(programmArt == artBaustelle && adresse == master)
  {
    //Sende ROT auf Ampel 1 und setze diese ebenfalls auf rot. Warte dann 5 Sek.
    SendeText("SETZE ROT", 1);
    SetzeRot(5000);
    //Diese Ampel bekommt zuerst grün!
    SetzeGelbRot(blinkdauer);
    SendeText("SETZE ROT", 1);
    //Warte 25 Sek, setze dann wieder rot...
    SetzeGruen(25000);
    SendeText("SETZE ROT", 1);
    SetzeGelb(blinkdauer);
    SendeText("SETZE ROT", 1);
    SetzeRot(20000);
    //... für 20 Sek. sind beide Ampeln also rot. Dann bekommt die andere Ampel grün
    //nach gleichem Prinzip.
    SendeText("SETZE GELB-ROT", 1);
    Delay(blinkdauer);
    SendeText("SETZE GRUEN", 1);
    Delay(25000);
    SendeText("SETZE GELB", 1);
    Delay(blinkdauer);
    SendeText("SETZE ROT", 1);
    //Rot für 15 Sek, weil am Anfang Wartezeit von 5 Sek ist.
    Delay(15000);
  }
}

void SetzeRot(int dauer)
{
  //mache alle anderen aus, rot an und warte für dauer
  Serial.print("Die Ampel ist fuer ");
  Serial.print(dauer / 1000);
  Serial.println(" Sekunde(n) rot.");
  aus(autoGelb);
  aus(autoGruen);
  an(autoRot);
  Delay(dauer);
}
void SetzeRot()
{
  Serial.println("Die Ampel ist rot.");
  //wie oben ohne Dauer
  aus(autoGelb);
  aus(autoGruen);
  an(autoRot);
}

void SetzeGelbRot(int dauer)
{
  Serial.print("Die Ampel ist fuer ");
  Serial.print(dauer / 1000);
  Serial.println(" Sekunde(n) gelb-rot.");
  //Grün aus, Rest an fuer dauer
  aus(autoGruen);
  an(autoRot);
  an(autoGelb);
  Delay(dauer);
}

void SetzeGelbRot()
{
  Serial.println("Die Ampel ist gelb-rot");
  //wie oben ohne dauer
  aus(autoGruen);
  an(autoRot);
  an(autoGelb);
}

void SetzeGruen(int dauer)
{
  Serial.print("Die Ampel ist fuer ");
  Serial.print(dauer / 1000);
  Serial.println(" Sekunde(n) gruen.");
  //Alles außer grün aus, dauer
  aus(autoRot);
  aus(autoGelb);
  an(autoGruen);
  Delay(dauer);
}

void SetzeGruen()
{
  Serial.println("Die Ampel ist gruen");
  //s.o. ohne dauer
  aus(autoRot);
  aus(autoGelb);
  an(autoGruen);
}

void SetzeGelb(int dauer)
{
  Serial.print("Die Ampel ist fuer ");
  Serial.print(dauer / 1000);
  Serial.println(" Sekunde(n) gelb.");
  //s.o. mit gelb, dauer
  aus(autoRot);
  aus(autoGruen);
  an(autoGelb);
  Delay(dauer);
}

void SetzeGelb()
{
  Serial.println("Die Ampel ist gelb.");
  //s.o. ohne dauer
  aus(autoRot);
  aus(autoGruen);
  an(autoGelb);
}
/**** Kommunikation ****
1. Master()
2. Anweisungen()
3. Empfangen()
4. ProgrammAufrufen(string)
5. SendeText(string, adresse)
***********************/
//Variablen für die Folgemethoden.
char c = ' ';
char* converted_c;
char* zahlenworte[]={"NULL", "EINS", "ZWEI", "DREI", "VIER", "FUENF","SECHS","SIEBEN","ACHT","NEUN"};
void Master()
{
  //Beginne I²C-Verbindung
  //Wire.begin();
  //Wenn die Konsolenverbindung verfügbar ist (> 0), dann...
  if(Serial.available() > 0)
  {
    //Beginne Übertragung zu Gerät 1
    //Wire.beginTransmission(1);
    //Während die Verbindung zur Konsole verfügbar ist, ...
    while(Serial.available() > 0)
    {
      //Lese das Zeichen vom PC, 
      c = Serial.read();
      //Wenn ein gültiges Zeichen, 
      if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 32 || c == 45)
       {
         //Aber Kleinbuchstabe, dann mach einen Großbuchstaben drauß
         if (c >= 97 && c <= 122)
         {
           c = c - 32;
         }
       }
      //Wenn eine Zahl, 
      if(c >= 48 && c <= 57)
      {
        //mache eine vernünftige draus, 
        c = c - 48;
        //wandle sie um
        converted_c = zahlenworte[c];
        //und sende dann das Chararray per I²C
        //Wire.write(converted_c);
        //Füge dann dem Anweisungstext die Zeichenfolge hinzu.
        anweisung += converted_c;
      }
      //Keine Zahl?
      else 
      {
        //Verschicke das Zeichen so und füge es der Anweisung hinten dran.
        //Wire.write(c);
        anweisung += c;
      }
    }
    //Beende dann am Ende die Übertragung
    //Wire.endTransmission(1);
  }
  //Wenn die Anweisung etwas enthält, ...
  if(anweisung != "")
  {
    //Rufe das entsprechende Programm per String auf...
    ProgrammAufrufen(anweisung, true);
    //... und setze die Anweisung auf leer.
    anweisung = "";
  }
}

void Anweisungen()
{
  //Höre auf die Adresse x
  //Wire.begin(adresse);
  //Und setze die Methode
  Wire.onReceive(Empfangen);
}
void Empfangen(int anzahl)
{
  //Leere die Anweisung
  anweisung = "";
  //Während eine Anweisung per I2C kommt...
  while(Wire.available())
  {
    //Lese jedes Char, überprüfe es auf Gültigkeit und ändere ungültige Symbole
    c = Wire.read();
     if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 32 || c == 45)
     {
       if (c >= 97 && c <= 122)
       {
         c = c - 32;
       }
     }
     //Füge der Anweisung das Char hinzu
     anweisung += c;
  }
  //Wenn die Anweisung nicht leer ist, rufe das Programm per String auf und lösche den Anweisungsstring
  if(anweisung != "")
    {
      ProgrammAufrufen(anweisung, false);
      anweisung = "";
    }
}

void ProgrammAufrufen(String text, boolean gesendet)
{
  if(gesendet) Serial.print("Gesendet: ");
  else Serial.print("Empfangen: ");
  Serial.println(text);
  
  if(text == "BAUSTELLENAMPEL" && adresse == master)
  {
    programmArt = artBaustelle;
    BaustellenAmpel();
  }
  else if(text == "BLINKEN" && adresse == master)
  {
    programmArt = artBlinken;
    Blinken();
  }
  else if(text == "AUS")
  {
    programmArt = artAus;
    alleAus();
  }
  else if(text == "SETZE ROT" && adresse != master)
    SetzeRot();
  else if(text == "SETZE GELB-ROT" && adresse != master)
    SetzeGelbRot();
  else if(text == "SETZE GELB" && adresse != master)
    SetzeGelb();
  else if(text == "SETZE GRUEN" && adresse != master)
    SetzeGruen();
  else if (text == "SETZE PERIODENDAUER EINS" && adresse == master)
    Periodendauer(1);
  else if (text == "SETZE PERIODENDAUER ZWEI" && adresse == master)
    Periodendauer(2);
  else if (text == "SETZE PERIODENDAUER DREI" && adresse == master)
    Periodendauer(3);
  else if (text == "SETZE PERIODENDAUER VIER" && adresse == master)
    Periodendauer(4);
  else if (text == "SETZE PERIODENDAUER FUENF" && adresse == master)
    Periodendauer(5);
  else if (text == "SETZE PERIODENDAUER SECHS" && adresse == master)
    Periodendauer(6);
  else if (text == "SETZE PERIODENDAUER SIEBEN" && adresse == master)
    Periodendauer(7);
  else if (text == "SETZE PERIODENDAUER ACHT" && adresse == master)
    Periodendauer(8);
  else if (text == "SETZE PERIODENDAUER NEUN" && adresse == master)
    Periodendauer(9);
}

void SendeText(char* text, int zielAdresse)
{
    //Text per I²C an Empfänger senden
    Wire.beginTransmission(zielAdresse);
    Wire.write(text);
    //Info auf Konsole ausgeben
    Serial.print("Gesendet: ");
    Serial.println(text);
    Wire.endTransmission(zielAdresse);
}
/**** Hilfsmethoden ****
* 1. Delay(int dauer)
* 2. PinModesSetzen()
* 3. an(int pin)
* 4. aus(int pin)
* 5. (int)AdresseLesen()
* 6. AlleAus()
***********************/
//1
void Delay(int dauer)
{
  //Eigene DELAY-Methode: Zeigt die Dauer des Wartens an,
  if(programmArt != artAus)
  {
    Serial.print("Warte: ");
    Serial.print(dauer / 1000);
    Serial.println(" Sekunde(n)");
  }
  for(int i = 0; i<=dauer;i++)
  {
    delay(1);
    //Liest in jedem 30. Durchlauf (etwa nach 30 Millisekunden)..
    if(i%30 == 0)
    {
      //Prüft nach Master und führt dann die Master-Methode aus
      if(adresse == master)
        Master();
      //Oder sonst die Slave-Methode
      else Anweisungen();
    }
  }
}
//2
void PinModesSetzen()
{
  //Setzt die OUTPUTS und die INPUTS
  pinMode(pinImmerAn, OUTPUT);
  pinMode(autoRot, OUTPUT);
  pinMode(autoGelb, OUTPUT);
  pinMode(autoGruen, OUTPUT);
  pinMode(fussRot, OUTPUT);
  pinMode(fussGruen, OUTPUT);
  pinMode(dip1, INPUT_PULLUP);
  pinMode(dip2, INPUT_PULLUP);
  pinMode(dip3, INPUT_PULLUP);
  pinMode(dip4, INPUT_PULLUP);
  pinMode(bedarfsTaster, INPUT_PULLUP);
  //Schaltet PULLUP-Wiederstände an
  digitalWrite(dip1, HIGH);
  digitalWrite(dip2, HIGH);
  digitalWrite(dip3, HIGH);
  digitalWrite(dip4, HIGH);
  digitalWrite(bedarfsTaster, HIGH);
}
//3
void an(int pin)
{
  //an(pin): eigene Kurzschreibweise für...
  digitalWrite(pin, HIGH);
}
//4
void aus(int pin)
{
  //s.o.
  digitalWrite(pin, LOW);
}
//5
int AdresseLesen()
{
  //Liest die Adresse anhand der DIP-Schalter
  if(digitalRead(dip1) == LOW)
    return 1;
  else if(digitalRead(dip2) == LOW)
    return 2;
  else if(digitalRead(dip3) == LOW)
    return 3;
  else if(digitalRead(dip4) == LOW)
    return 4;
  //Wenn keiner gedrückt, ist MASTER
  else return master;
}
//6
void alleAus()
{
  //Alle LEDs gehen aus..
  digitalWrite(autoRot, LOW);
  digitalWrite(autoGelb, LOW);
  digitalWrite(autoGruen, LOW);
  digitalWrite(fussRot, LOW);
  digitalWrite(fussGruen, LOW);
}
