#include <Arduino.h>
#include <SoftwareSerial.h>
#include <string.h>
#include "MzMacros.h"

SoftwareSerial espSerial(3, 6); // RX, TX

#define __BAUD_RATE 115200
#define ESP_READ_PIN 13
#define LED_PIN 9

void setup() {
  pinMode(ESP_READ_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(__BAUD_RATE);
  espSerial.begin(__BAUD_RATE);

  while(!Serial) {};
  while(!espSerial) {};
}

#define _LOOP_DELAY 250
void loop() {
  int serialBytes = espSerial.available();
  if (serialBytes > 0) {
    char buffer[serialBytes];
    espSerial.readBytes(buffer, serialBytes);
    debugPrint("esp> ", buffer);
  }

  int espSignal = digitalRead(ESP_READ_PIN);
  // debugPrint("espSignal: ", espSignal);

  int ledOn = espSignal == 0;
  digitalWrite(LED_PIN, ledOn);

  delay(_LOOP_DELAY);
}


