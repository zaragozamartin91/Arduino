#include <Arduino.h>

const int READ_PIN = PIN_A5;
const int WRITE_PIN = PIN3;
const float MAX_VOLTS = 5.;

float normalize_value(float);

char* floatToString(float);

void setup()
{
  pinMode(READ_PIN, INPUT);
  pinMode(WRITE_PIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial)
  {
  };
  Serial.println("Serial comm started!");
}

void loop()
{
  float analog_value = analogRead(READ_PIN); // value from 0 to 1023.
  

  // We have to write an analog voltage to the WRITE_PIN
  // The write has to be between 0 and 255
  /* 
  1023 -> 255
  rv   -> ?

  thus 255 * rv / 1023 = ?
  */

  float normalized_value = normalize_value(analog_value);

  // Serial.print("analog_value: ");
  // Serial.print(analog_value);
  // Serial.print(" ; normalized_value: ");
  // Serial.println(normalized_value);

  char* analog_value_str = floatToString(analog_value);
  char* normalized_value_str = floatToString(normalized_value);
  char buffer[64];
  sprintf(buffer, "analog_value: %s ; normalized_value: %s", analog_value_str, normalized_value_str);
  Serial.println(buffer);

  int i_normalized_value = static_cast<int>(normalized_value);

  analogWrite(WRITE_PIN, i_normalized_value);

  delete[] analog_value_str;
  delete[] normalized_value_str;
  delay(1000);
}


float normalize_value(float analog_value) {
  return 255. * analog_value / 1023.;
}

char* floatToString(float value) {
  char* buffer = new char[8];
  dtostrf(value, 0, 2, buffer);
  return buffer;
}