#include <Arduino.h>


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  while(!Serial) {};
}

#define _HTIME 500
#define _LTIME 400
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(_HTIME);
  digitalWrite(LED_BUILTIN, LOW);
  delay(_LTIME);
  Serial.println("Looping");
}

