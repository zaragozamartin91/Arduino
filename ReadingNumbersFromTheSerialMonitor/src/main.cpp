#include <Arduino.h>
#include <math.h>

int WRITE_PIN = PIN3;
long MAX_INPUT = 100;
char STR_BUFFER[128];

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial)
  {
  };

  pinMode(WRITE_PIN, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  sprintf(STR_BUFFER, "Write a power input between 0 and %ld : ", MAX_INPUT);
  Serial.print(STR_BUFFER);

  // waiting for input & reding power_input
  while (Serial.available() == 0)
  {
  };
  long power_input = Serial.parseInt();

  sprintf(STR_BUFFER, "Your power input is %ld isn't it?", power_input);
  Serial.println(STR_BUFFER);
  Serial.println();

  if (power_input < 0 || power_input > MAX_INPUT)
  {
    Serial.println("Invalid input try again");
  }
  else
  {
    float f_power = 255. * static_cast<float>(power_input) / static_cast<float>(MAX_INPUT);
    int i_power = static_cast<int>(f_power);
    analogWrite(WRITE_PIN, i_power);
  }

}
