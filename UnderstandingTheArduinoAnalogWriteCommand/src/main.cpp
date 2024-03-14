#include <Arduino.h>

// From video https://www.youtube.com/watch?v=YfV-vYT3yfQ&list=PLGs0VKk2DiYw-L-RibttcvK-WBZm8WLEP&index=8

int red_pin = 9;
int led_brightness = 64; // value ranges from 0 to 255

void setup()
{
  // put your setup code here, to run once:
  pinMode(red_pin, OUTPUT);
  Serial.begin(9600);
  while (!Serial)
  {
  }
  Serial.println("Serial comm established");
}

void loop()
{
  // put your main code here, to run repeatedly:
  analogWrite(red_pin, led_brightness);
}
