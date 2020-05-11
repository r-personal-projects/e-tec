#define DAUER 100
void setup()
{
	for (int i = 2; i <= 12; i = i + 2)
		pinMode(i, OUTPUT);
}

void loop()
{
	for (int i = 2; i <= 12; i = i + 2)
	{
		digitalWrite(i, HIGH);
		delay(DAUER);
		digitalWrite(i, LOW);
	}
	for (int i = 12; i >= 2; i = i - 2)
	{
		digitalWrite(i, HIGH);
		delay(DAUER);
		digitalWrite(i, LOW);
	}
}