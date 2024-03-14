#include <Arduino.h>

const int READ_PIN = PIN_A5;
const float MAX_VOLTS = 5.;
const int RED_LED_PIN = PIN4;
const float ALERT_VALUE = 4.;

float to_volts(float);
void alert(float);

void setup()
{
  pinMode(READ_PIN, INPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial)
  {
  };
  Serial.println("Serial comm started!");
}

void loop()
{
  float analog_value = to_volts(analogRead(READ_PIN));
  char analog_value_str[8];
  dtostrf(analog_value, 4, 2, analog_value_str);

  char buffer[64];
  sprintf(buffer, "Value: %s", analog_value_str);
  Serial.println(buffer);

  alert(analog_value);

  delay(500);
}

float to_volts(float read_value)
{
  return MAX_VOLTS * read_value / 1023.;
}

void alert(float analog_value)
{
  if (analog_value > ALERT_VALUE)
  {
    digitalWrite(RED_LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(RED_LED_PIN, LOW);
  }
}