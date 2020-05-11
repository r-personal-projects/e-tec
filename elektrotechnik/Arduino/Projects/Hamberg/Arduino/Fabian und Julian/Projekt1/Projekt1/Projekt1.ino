#define rot1 2
#define gruen1 4
#define gelb1 6
#define rot2 8
#define gruen2 10
#define gelb2 12


void setup()
{
	pinMode(rot1, OUTPUT);
	pinMode(gruen1, OUTPUT);
	pinMode(gelb1, OUTPUT);
	pinMode(rot2, OUTPUT);
	pinMode(gruen2, OUTPUT);
	pinMode(gelb2, OUTPUT);
}


void loop()
{
	digitalWrite(rot1, HIGH);
	delay(1000);
	digitalWrite(rot1, LOW);

	digitalWrite(gruen1, HIGH);
	delay(1000);
	digitalWrite(gruen1, LOW);

	digitalWrite(gelb1, HIGH);
	delay(1000);
	digitalWrite(gelb1, LOW);

	digitalWrite(rot2, HIGH);
	delay(1000);
	digitalWrite(rot2, LOW);

	digitalWrite(gruen2, HIGH);
	delay(1000);
	digitalWrite(gruen2, LOW);

	digitalWrite(gelb2, HIGH);
	delay(1000);
	digitalWrite(gelb2, LOW);
	delay(1000);

	digitalWrite(gelb2, HIGH);
	delay(1000);
	digitalWrite(gelb2, LOW);

	digitalWrite(gruen2, HIGH);
	delay(1000);
	digitalWrite(gruen2, LOW);

	digitalWrite(rot2, HIGH);
	delay(1000);
	digitalWrite(rot2, LOW);

	digitalWrite(gelb1, HIGH);
	delay(1000);
	digitalWrite(gelb1, LOW);

	digitalWrite(gruen1, HIGH);
	delay(1000);
	digitalWrite(gruen1, LOW);

	digitalWrite(rot1, HIGH);
	delay(1000);
	digitalWrite(rot1, LOW);
	delay(1000);
}	