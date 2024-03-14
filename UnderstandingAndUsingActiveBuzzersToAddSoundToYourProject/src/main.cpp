#include <Arduino.h>

const int BUZZER_PIN = PIN6;
const int READER_PIN = PIN_A3;
const long BAUD_RATE = 9600;
const int BUZZER_POWER = 64;
const int BUZZER_THRESHOLD = 600;

void setup()
{
  /* Active buzzers are simple to use. They expect a DC current and emit a sound */
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(READER_PIN, INPUT);

  Serial.begin(BAUD_RATE);
  while (!Serial)
  {
  }
}

void loop()
{
  for (int powerValue = analogRead(READER_PIN); powerValue > BUZZER_THRESHOLD; powerValue = analogRead(READER_PIN))
  {
    analogWrite(BUZZER_PIN, BUZZER_POWER);
  }

  analogWrite(BUZZER_PIN, 0);
  delay(1000);
}
