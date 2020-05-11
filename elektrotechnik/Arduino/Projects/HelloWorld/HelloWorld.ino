int dauer = 1;

int saeulen[] = {7,8,12,4,5,6,13,A4,A5};
int ebenen[] = {9,10,11};

void setup()
{
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
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
}

void loop() 
{
  for(int e = 0; e < 3; e++) 
  {  
    an(ebenen[e]);
    for(int s = 0; s < 9; s++) 
    {
      an(saeulen[s]);
      delay(dauer);
      aus(saeulen[s]);
    }
    aus(ebenen[e]);
  }
  if(dauer >= 500)
    dauer = 1;
  dauer = dauer * 2;
}

void an(int channel) 
{
  digitalWrite(channel, HIGH);
}

void aus(int channel)
{
  digitalWrite(channel, LOW);
}
