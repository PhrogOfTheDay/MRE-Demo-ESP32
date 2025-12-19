#include <Neza74HC165.h>
#include <Arduino.h>
#include <vector>
#include "ShiftIn.h"

/*
#include <Adafruit_NeoPixel.h>

int const LED_PIN = 23;
int const LED_COUNT = 17;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
*/

int PL = 18;
int CLK_CP = 19;
int CE = 21;
int DATA = 22;
const int numOfRegisters = 1;

const int numBits = numOfRegisters * 8;
ShiftIn<1> shift;

Neza74HC165<numOfRegisters> shiftRegs;
std::vector<int> prevRegState;

std::vector<int> getCurrentStates()
{
  // String day_of_the_week[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
  // String times_of_the_day[3] = {"Morning", "Afternoon", "Evening"};

  std::vector<int> states;

  shiftRegs.update(); // update the shift register

  for (int i = 0; i < 8 * numOfRegisters; i++)
  {

    int bitVal = shiftRegs.read(i) ? 1 : 0; // read single bit
    Serial.print(shiftRegs.read(i));
    if (bitVal == 1)
    {
      Serial.println("Detected HIGH at bit: " + (String)i);
      states.push_back(i);
      // Serial.println("Number: " + (String)i);
      // Serial.println("Day: " + day_of_the_week[i / 3]);
      // Serial.println("Time: " + times_of_the_day[i % 3]);
    }
  }

  return states;
}

void setup()
{
  Serial.begin(115200);
  shiftRegs.begin(DATA, PL, CLK_CP);
  shift.begin(PL, CE, DATA, CLK_CP);

  // prevRegState = getCurrentStates();
  //  strip.begin();
  //  strip.show();
}

bool detectUpdate()
{
  std::vector<int> currentRegState = getCurrentStates();
  if (currentRegState != prevRegState)
  {
    prevRegState = currentRegState;
    return true;
  }
  return false;
}

void displayValues()
{
  // print out all 8 buttons
  for (int i = 0; i < shift.getDataWidth(); i++)
    Serial.print(shift.state(i)); // get state of button i
  Serial.println();
}

void loop()
{
  /*
  if (detectUpdate())
  {
    Serial.println("State changed!");
    turnOnOrOffLED(prevRegState.back());
  }*/
  /*getCurrentStates();
  Serial.println("");
  delay(3000);*/
  if (shift.update())
  {
    Serial.println("I got hit");
    displayValues();
  }
  delay(1);
}
