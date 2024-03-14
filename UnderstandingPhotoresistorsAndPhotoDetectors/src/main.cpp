#include <Arduino.h>

const int VOLTAGE_PIN=PIN_A5;
const int MAX_READ=1023;
const int MAX_VOLTS=5;
const int GREEN_PIN=PIN5;
const int RED_PIN=PIN3;

float mapToVolts(int voltageRead);

void setup() {
  pinMode(VOLTAGE_PIN, INPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  Serial.begin(9600);
  while(!Serial) {}
}

void loop() {
  int voltageRead=analogRead(VOLTAGE_PIN);
  Serial.print("voltageRead: "); Serial.println(voltageRead);

  float voltsValue = mapToVolts(voltageRead);
  Serial.print("voltsValue: "); Serial.println(voltsValue);

  boolean lightsOnDetected = voltsValue > 1.0;
  boolean lightsOffDetected = !lightsOnDetected;

  digitalWrite(GREEN_PIN, lightsOnDetected);
  digitalWrite(RED_PIN, lightsOffDetected);

  delay(500);
}

float mapToVolts(int voltageRead) {
  return static_cast<float>(MAX_VOLTS) * static_cast<float>(voltageRead) / static_cast<float>(MAX_READ);
}