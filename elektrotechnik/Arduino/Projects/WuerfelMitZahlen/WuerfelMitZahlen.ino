int Dauer = 1000;

int Ebenen[] = {
  10,11,12}; //Ebenen von oben nach unten
int Saeulen[] = {
  7,8,9,4,5,6,13, 0,0}; //Säulen nach Reihenfolge
/*

 .(9)   .(6)   .(xx)  --- 1
 .(8)   .(5)   .(xx)  --- 2
 .(7)   .(4)   .(13)  --- 3
 
 7 = Saeulen[0]
 8 = Saeulen[1]
 9 = Saeulen[2]
 4 = Saeulen[3]
 5 = Saeulen[4]
 6 = Saeulen[5]
 13 = Saeulen[6]
 xx = Saeulen[7]
 xx = Saeulen[8]
 
 */

void setup()
{
  //PinModesEinstellen
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
}

void loop()
{
  //nummerAnzeigen(3, 1) -- nummer, ebene
  //delay(Dauer)
  //zuruecksetzen()
  nummernAnzeigen(1,2,3);
  nummernAnzeigen(2,3,5);
  nummernAnzeigen(3,5,6);
  nummernAnzeigen(5,6,9);
  nummernAnzeigen(6,9,1);
  nummernAnzeigen(9,1,2);
}

void nummernAnzeigen(int oben, int mitte, int unten) 
{
  for(int i = 0; i <= Dauer; i++)
  {
    int zeit = 1;
    zuruecksetzen();
    delay(zeit);
    nummerAnzeigenEinzeln(oben,1);
    delay(zeit);
    zuruecksetzen();
    delay(zeit);
    nummerAnzeigenEinzeln(mitte,2);
    delay(zeit);
    zuruecksetzen();
    delay(zeit);
    nummerAnzeigenEinzeln(unten,3);
    delay(zeit);
    zuruecksetzen();
  }
}

void zuruecksetzen()
{
  for(int i = 9; i > 0; i--) {
    aus(Saeulen[i]); 
  }
  for(int i = 3; i > 0; i--) {
    aus(Ebenen[i]); 
  }
}

void nummerAnzeigenEinzeln(int nummer, int ebene)
{
  switch(nummer)
  {
  case 0:
    nullAnzeigen(ebene);
    break;
  case 1:
    einsAnzeigen(ebene);
    break;
  case 2:
    zweiAnzeigen(ebene);
    break;
  case 3:
    dreiAnzeigen(ebene);
    break;
  case 4:
    vierAnzeigen(ebene);
    break;
  case 5:
    fuenfAnzeigen(ebene);
    break;
  case 6:
    sechsAnzeigen(ebene);
    break;
  case 7:
    siebenAnzeigen(ebene);
    break;
  case 8:
    achtAnzeigen(ebene);
    break;
  case 9:
    neunAnzeigen(ebene);
    break;
  }
}



/*
.(2)   .(5)   .(8)  --- 1
 .(1)   .(4)   .(7)  --- 2
 .(0)   .(3)   .(6)  --- 3
 */
void nullAnzeigen(int ebene)
{
  an(Ebenen[ebene-1]);
  //unten rechts wg. null
  an(Saeulen[6]);
  an(Ebenen[ebene-1]);
}
void einsAnzeigen(int ebene)
{
  an(Ebenen[ebene-1]);
  an(Saeulen[4]);
}
void zweiAnzeigen(int ebene)
{
  an(Ebenen[ebene-1]);
  an(Saeulen[2]);
  an(Saeulen[6]);
}
void dreiAnzeigen(int ebene)
{
  an(Ebenen[ebene-1]);
  an(Saeulen[2]);
  an(Saeulen[4]);
  an(Saeulen[6]);
}
void vierAnzeigen(int ebene)
{
  an(Ebenen[ebene-1]);
  an(Saeulen[0]);
  an(Saeulen[2]);
  an(Saeulen[8]);
  an(Saeulen[6]);
}
void fuenfAnzeigen(int ebene)
{
  an(Ebenen[ebene-1]);
  an(Saeulen[0]);
  an(Saeulen[2]);
  an(Saeulen[8]);
  an(Saeulen[6]);
  an(Saeulen[4]);
}
void sechsAnzeigen(int ebene)
{
  an(Ebenen[ebene-1]);
  an(Saeulen[0]);
  an(Saeulen[1]);
  an(Saeulen[2]);
  an(Saeulen[5]);
  an(Saeulen[6]);
  an(Saeulen[7]);
}
void siebenAnzeigen(int ebene)
{
  an(Ebenen[ebene-1]);
  an(Saeulen[0]);
  an(Saeulen[1]);
  an(Saeulen[2]);
  an(Saeulen[5]);
  an(Saeulen[6]);
  an(Saeulen[7]);
  an(Saeulen[4]);
}
void achtAnzeigen(int ebene)
{
  an(Ebenen[ebene-1]);
  an(Saeulen[0]);
  an(Saeulen[1]);
  an(Saeulen[2]);
  an(Saeulen[3]);
  an(Saeulen[5]);
  an(Saeulen[6]);
  an(Saeulen[7]);
  an(Saeulen[8]);
}
void neunAnzeigen(int ebene)
{
  an(Ebenen[ebene-1]);
  an(Saeulen[0]);
  an(Saeulen[1]);
  an(Saeulen[2]);
  an(Saeulen[3]);
  an(Saeulen[5]);
  an(Saeulen[6]);
  an(Saeulen[7]);
  an(Saeulen[8]);
}








void an(int channel)
{
  digitalWrite(channel, HIGH);
}

void aus(int channel)
{
  digitalWrite(channel, LOW);
}

