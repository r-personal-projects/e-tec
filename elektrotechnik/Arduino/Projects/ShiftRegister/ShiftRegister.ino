#include <CryptoIO.h>


int sh = 8;
int st = 9;
int ds = 10;
int data[] = {1,1, 0,0,0,0, 0,0,1,1};

void setup() {
  // put your setup code here, to run once:
  pinMode(sh, OUTPUT);
  pinMode(st, OUTPUT);
  pinMode(ds, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(st, 0);
  for (int i = 0; i <= 9; i++) {
    digitalWrite(sh, 0);

    digitalWrite(ds, data[i]);

    delay(20);
    digitalWrite(sh, 1);
    delay(20);
  }

  digitalWrite(st, 1);

}
