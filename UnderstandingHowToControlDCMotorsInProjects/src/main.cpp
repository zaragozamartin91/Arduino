#include <Arduino.h>
#include "MyMacros.h"

#define _POTEN_PIN A5
#define _MOTOR_PIN 11

void setup() {
  pinMode(_POTEN_PIN, INPUT);
  pinMode(_MOTOR_PIN, OUTPUT);

  Serial.begin(9600);
  while(!Serial) {};
}

#define _LOOP_PERIOD 250
void loop() {
  auto potenRead = analogRead(_POTEN_PIN);

  // debugPrintArgs("potenRead: ", potenRead);

  auto motorPower = (255. * static_cast<float>(potenRead)) / 1023.;
  debugPrintArgs("motorPower: ", motorPower);

  analogWrite(_MOTOR_PIN, motorPower);

  delay(_LOOP_PERIOD);
}

