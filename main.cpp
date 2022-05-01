#include <Arduino.h>

int latchPin = 9;
int clockPin = 8;
int dataPin = 10;
int resetButton = 7;

byte dispData[] = {B00000000,
                   B01100000,
                   B11011010,
                   B11110010,
                   B01100110,
                   B10110110,
                   B10111110,
                   B11100000,
                   B11111110,
                   B11110110,
                   B11111100};

int currentNum[10];

unsigned long currentTime;
unsigned long resetTime;
bool lastButtonState;
unsigned long x;

void setup()
{
  Serial.begin(115200);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(resetButton, INPUT);
  lastButtonState = digitalRead(resetButton);
}

void RSTsequence()
{
  //
  digitalWrite(latchPin, LOW);
  for (x = 0; x <= 10; x++)
  {
    shiftOut(dataPin, clockPin, LSBFIRST, B00000000);
  }
  digitalWrite(latchPin, HIGH);

  delay(500);

  //  .
  digitalWrite(latchPin, LOW);
  for (x = 0; x <= 9; x++)
  {
    shiftOut(dataPin, clockPin, LSBFIRST, B00000000);
  }
  shiftOut(dataPin, clockPin, LSBFIRST, B00000001);
  digitalWrite(latchPin, HIGH);

  delay(500);

  // ..
  digitalWrite(latchPin, LOW);
  for (x = 0; x <= 8; x++)
  {
    shiftOut(dataPin, clockPin, LSBFIRST, B00000000);
  }
  shiftOut(dataPin, clockPin, LSBFIRST, B00000001);
  shiftOut(dataPin, clockPin, LSBFIRST, B00000001);
  digitalWrite(latchPin, HIGH);

  delay(500);

  //...
  digitalWrite(latchPin, LOW);
  for (x = 0; x <= 7; x++)
  {
    shiftOut(dataPin, clockPin, LSBFIRST, B00000000);
  }
  shiftOut(dataPin, clockPin, LSBFIRST, B00000001);
  shiftOut(dataPin, clockPin, LSBFIRST, B00000001);
  shiftOut(dataPin, clockPin, LSBFIRST, B00000001);
  digitalWrite(latchPin, HIGH);

  delay(500);

  //
  digitalWrite(latchPin, LOW);
  for (x = 0; x <= 10; x++)
  {
    shiftOut(dataPin, clockPin, LSBFIRST, B00000000);
  }
  digitalWrite(latchPin, HIGH);

  delay(500);

  //  0
  digitalWrite(latchPin, LOW);
  for (x = 0; x <= 9; x++)
  {
    shiftOut(dataPin, clockPin, LSBFIRST, B00000000);
  }
  shiftOut(dataPin, clockPin, LSBFIRST, dispData[10]);
  digitalWrite(latchPin, HIGH);

  delay(500);
}

void loop()
{
  if (digitalRead(resetButton) == HIGH && lastButtonState == false)
  {
    Serial.println("##### Reset Butten Pressed #####");
    RSTsequence();

    lastButtonState = true;
    resetTime = millis();
  }

  if (digitalRead(resetButton) == LOW && lastButtonState == true)
  {
    lastButtonState = false;
    Serial.println("##### Reset Butten Released #####");
  }

  currentTime = millis() - resetTime;

  currentNum[1] = currentTime % 10;
  currentNum[2] = (currentTime / 10) % 10;
  currentNum[3] = (currentTime / 100) % 10;
  currentNum[4] = (currentTime / 1000) % 10;
  currentNum[5] = (currentTime / 10000) % 10;
  currentNum[6] = (currentTime / 100000) % 10;
  currentNum[7] = (currentTime / 1000000) % 10;
  currentNum[8] = (currentTime / 10000000) % 10;
  currentNum[9] = (currentTime / 100000000) % 10;

  for (x = 1; x <= 9; x++)
  {
    if (currentNum[x] == 0 && currentTime >= pow(10, x))
    {
      currentNum[x] = 10;
    }
  }

  // latchPin low um flackern der LEDs zu verhindern
  digitalWrite(latchPin, LOW);
  // Daten senden in MSBFIRST Reihenfolge (alternativ LSBFIRST)
  for (x = 9; x >= 1; x--)
  {
    shiftOut(dataPin, clockPin, LSBFIRST, dispData[currentNum[x]]);
  }
  // latchPin high um Daten 'anzuzeigen'
  digitalWrite(latchPin, HIGH);
}