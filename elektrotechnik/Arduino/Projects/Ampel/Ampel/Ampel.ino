#include <Wire.h>

//Variablen
  //PINS
    //Rubeen
      //AmpelAusgänge
      const int autoRot = 6;
      const int autoGelb = 5;
      const int autoGruen = 4;
      const int fussRot = 3;
      const int fussGruen = 2;
      //2. Stromquelle
      const int pinImmerAn = A0;
      //DIP-Schalter
      const int dip1 = 7;
      const int dip2 = 8;
      const int dip3 = 9;
      const int dip4 = 10;
      const int bedarfsTaster = 11;
 //Adressen der anderen Ampeln
   const int hauptStrasse1 = 1;
   const int hauptStrasse2 = 2;
   const int nebenStrasse1 = 3;
   const int nebenStrasse2 = 4;
   const int masterAdresse = 0;
 //Laufzeitvariablen
   boolean master = false;
   int adresse = masterAdresse;
   String anweisung = "";
   boolean bedarf = false;
   
void setup()
{
  PinModesSetzen();
  adresse = AdresseLesen();
  Wire.begin(adresse);
  Wire.onReceive(WireEmpfangen);
  Serial.begin(9600);
  Serial.print("---- Arduino gestartet mit Adresse: ");
  Serial.print(adresse);
  Serial.println(" ----");
}

void loop()
{
  serialEvent();
  delay(500);
}
//------- Programmmethoden ---------//
void AutoRot()
{
  an(autoRot);
  Serial.println("Rote LED an");
}

void AutoGelb()
{
  an(autoGelb);
  Serial.println("Gelbe LED an");
}

void AutoGruen()
{
  an(autoGruen);
  Serial.println("Gruene LED an");
}

void FussRot()
{
  an(fussRot);
  Serial.println("Gelbe LED an");
}

void FussGruen()
{
  an(fussGruen);
  Serial.println("Gelbe LED an");
}

void AlleAus()
{
  aus(autoRot);
  aus(autoGelb);
  aus(autoGruen);
  aus(fussRot);
  aus(fussGruen);
  Serial.println("Alle LEDS aus");
}
//------- Mastermethoden ----------//
void masterDefekt()
{
  WireSenden("AUS", hauptStrasse1);
  WireSenden("AUS", hauptStrasse2);
  WireSenden("AUS", nebenStrasse1);
  WireSenden("AUS", nebenStrasse2);
}

void masterAus()
{
  //Kommt noch...
}

void testAn()
{
  WireSenden("SETZE ROT", hauptStrasse1);
  WireSenden("SETZE ROT", hauptStrasse2);
  WireSenden("SETZE ROT", nebenStrasse1);
  WireSenden("SETZE ROT", nebenStrasse2);
}
//---------------------------------//
  //ProgrammHandler
  void Programmauswahl(String befehl)
  {
    Serial.println(anweisung);
    if(!master)
    {
      if(befehl == "SETZE GELB")
        AutoGelb();
      if(befehl == "SETZE ROT")
        AutoRot();
      if(befehl == "SETZE GRUEN")
        AutoGruen();
      if(befehl == "SETZE F ROT")
        FussRot();
      if(befehl == "SETZE F GRUEN")
        FussGruen();
      if(befehl == "AUS")
        AlleAus();
    }
    else
    {
      if(befehl == "DEFEKT")
        masterDefekt();
      if(befehl == "AUS")
        masterAus();
      if(befehl == "TESTAN")
        testAn();
    }
  }
  //Empfangsmethode Serial
    //Variablen für die Folgemethoden.
      char c = ' ';
      char* converted_c;
      char* zahlenworte[]={"NULL", "EINS", "ZWEI", "DREI", "VIER", "FUENF","SECHS","SIEBEN","ACHT","NEUN"};
  void serialEvent()
  {
    if(master)
    {
      while(Serial.available())
      {
        Serial.println(master);
          char c = Serial.read();          
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
            //Füge dann dem Anweisungstext die Zeichenfolge hinzu.
            anweisung += converted_c;
          }
          //Keine Zahl?
          else 
          {
            //Verschicke das Zeichen so und füge es der Anweisung hinten dran.
            anweisung += c;
          }
          
          delay(5);
      }
      
      if(anweisung != "")
      {
          //Anweisung verschicken
          Programmauswahl(anweisung);
          anweisung = "";
      }
    }
  }
  //Sendemethode Wire
  void WireSenden(String text, int ziel)
  {
    const char* n = text.c_str();
    Wire.beginTransmission(ziel);
    Wire.write(n);
    Serial.print("Gesendet an: ");
    Serial.print(ziel);
    Serial.print(" - ");
    Serial.println(text);
    Wire.endTransmission();
    delete [] n;
  }
  //Empfangsmethode Wire
  void WireEmpfangen(int howMany)
  {
    if(!master)
    {
      while(0 < Wire.available())
      {
        char c = Wire.read();
        
        //Auf gültiges Zeichen prüfen und zu Großbuchstaben machen
        if ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 32 || c == 45)
       {
         if (c >= 97 && c <= 122)
         {
           c = c - 32;
         }
       }
        
        anweisung += c;
      }
      Serial.print("Empfangen: ");
      Serial.println(anweisung);
      Programmauswahl(anweisung);
      anweisung = "";
    }
    else 
    {
      bedarf = true;
    }
  }
  //Setzt die OUTPUTS & INPUTS
  void PinModesSetzen()
  {
    //OUTPUTS definieren
      pinMode(autoRot, OUTPUT);
      pinMode(autoGelb, OUTPUT);
      pinMode(autoGruen, OUTPUT);
      pinMode(fussRot, OUTPUT);
      pinMode(fussGruen, OUTPUT);
      pinMode(pinImmerAn, OUTPUT);
      //INPUTS
      pinMode(dip1, INPUT_PULLUP);
      pinMode(dip2, INPUT_PULLUP);
      pinMode(dip3, INPUT_PULLUP);
      pinMode(dip4, INPUT_PULLUP);
      pinMode(bedarfsTaster, INPUT_PULLUP);
      //Pullupwiderstände aktivieren
      digitalWrite(dip1, HIGH);
      digitalWrite(dip2, HIGH);
      digitalWrite(dip3, HIGH);
      digitalWrite(dip4, HIGH);
      digitalWrite(bedarfsTaster, HIGH);
  }

  int AdresseLesen()
  {
    //Zustände des DIP-Schalters abfragen
    if(digitalRead(dip1) == LOW)
      return hauptStrasse1;
    if(digitalRead(dip2) == LOW)
      return hauptStrasse2;
    if(digitalRead(dip3) == LOW)
      return nebenStrasse1;
    if(digitalRead(dip4) == LOW)
      return nebenStrasse2;
    //Kein Schalter: Ampel ist MASTER
    else { master = true; return masterAdresse; }
  }
  
  void an(int pin)
  {
    digitalWrite(pin, HIGH);
  }
  void aus(int pin)
  {
    digitalWrite(pin, LOW);
  }
