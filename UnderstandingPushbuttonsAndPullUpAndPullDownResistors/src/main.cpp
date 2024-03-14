#include <Arduino.h>
#include "ToggleButton.h"

int READ_PIN=12;
int LED_PIN=LED_BUILTIN;

boolean v_ledOn = false;

void toggleLed(int pressCount) { v_ledOn = !v_ledOn; }
mz::ToggleButton v_toggle(&toggleLed);

int v_buttonRead;
int v_pressCount;

void setup() {
  pinMode(READ_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  while(!Serial){};
}

void loop() {
  v_buttonRead= digitalRead(READ_PIN);
  
  v_toggle.update(v_buttonRead);

  v_pressCount = v_toggle.getPressCount();

  Serial.print("v_pressCount: ");Serial.print(v_pressCount);Serial.print(" ; v_buttonRead: ");Serial.println(v_buttonRead);

  digitalWrite(LED_PIN, v_ledOn);

  delay(100);
}



// #include <Arduino.h>
// #include "ToggleButton.h"

// int READ_PIN=PIN4;
// int LED_PIN=PIN3;

// boolean v_ledOn = false;

// void dimLed(int);

// mz::ToggleButton v_toggle(&dimLed);

// int v_buttonRead;
// int v_pressCount;

// void setup() {
//   pinMode(READ_PIN, INPUT);
//   pinMode(LED_PIN, OUTPUT);

//   Serial.begin(9600);
//   while(!Serial){};
// }

// void loop() {
//   v_buttonRead= digitalRead(READ_PIN);
  
//   v_toggle.update(v_buttonRead);

//   v_pressCount = v_toggle.getPressCount();

//   Serial.print("v_pressCount: ");Serial.print(v_pressCount);Serial.print(" ; v_buttonRead: ");Serial.println(v_buttonRead);

//   digitalWrite(LED_PIN, v_ledOn);

//   delay(100);
// }

// void dimLed(int pressCount) { v_ledOn = !v_ledOn; }


