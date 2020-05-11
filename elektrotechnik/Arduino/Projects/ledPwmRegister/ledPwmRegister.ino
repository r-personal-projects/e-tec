

const int ShiftPWM_latchPin=9;
const bool ShiftPWM_invertOutputs = false; 
const bool ShiftPWM_balanceLoad = false;

const int ShiftPWM_dataPin = 10;
const int ShiftPWM_clockPin = 8;

#include <ShiftPWM.h>

unsigned char maxBrightness = 255;
unsigned char pwmFrequency = 75;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
  // Sets the number of 8-bit registers that are used.
  ShiftPWM.SetAmountOfRegisters(1);

  // SetPinGrouping allows flexibility in LED setup. 
  // If your LED's are connected like this: RRRRGGGGBBBBRRRRGGGGBBBB, use SetPinGrouping(4).
  ShiftPWM.SetPinGrouping(1); //This is the default, but I added here to demonstrate how to use the funtion
  ShiftPWM.Start(pwmFrequency,maxBrightness);
}

void loop() {
  // put your main code here, to run repeatedly:
  ShiftPWM.SetAll(1);
  //ShiftPWM.PrintInterruptLoad();
  //ShiftPWM.OneByOneSlow();
}
