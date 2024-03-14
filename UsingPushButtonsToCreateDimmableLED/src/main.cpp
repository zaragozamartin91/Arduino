#include <Arduino.h>
#include "ToggleButton.h"

int READ_PIN=PIN4;
int LED_PIN=PIN3;

int LED_POWER_LEVELS[] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 255};
int LED_POWER_LEVELS_L = sizeof(LED_POWER_LEVELS) / sizeof(LED_POWER_LEVELS[0]);

int v_ledPowerLevel = 0;

/* Declaring Toggle button */
void dimLed(int pressCount) {
  int powerLevelIndex = pressCount % LED_POWER_LEVELS_L;
  v_ledPowerLevel = LED_POWER_LEVELS[powerLevelIndex];
}
int readButtonSignal() {return digitalRead(READ_PIN);}
mz::ToggleButton v_toggle(&dimLed, &readButtonSignal);

int v_pressCount = 0;

void setup() {
  pinMode(READ_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  while(!Serial){};
}

void loop() {
  v_toggle.update();

  v_pressCount = v_toggle.getPressCount();
  Serial.print("v_pressCount: ");Serial.print(v_pressCount);Serial.print(" ; v_ledPowerLevel: ");Serial.println(v_ledPowerLevel);

  analogWrite(LED_PIN, v_ledPowerLevel);

  delay(100);
}