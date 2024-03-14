#include <Arduino.h>

int built_in_pin = 13;

void setup()
{
  // put your setup code here, to run once:
  pinMode(built_in_pin, OUTPUT); // pin 13 is connected to board led
  Serial.begin(9600);
  while (!Serial)
    ; 
  Serial.println("Serial com started!");
}

void loop()
{
  // put your main code here, to run repeatedly:
  // digitalWrite(built_in_pin, HIGH);
  // delay(900);
  // Serial.println("Hello there!");
  // digitalWrite(built_in_pin, HIGH);
  Serial.println("My name is PEPE!");
  delay(2000);
  digitalWrite(built_in_pin, LOW);
  delay(2000);
  digitalWrite(built_in_pin, HIGH);
}
