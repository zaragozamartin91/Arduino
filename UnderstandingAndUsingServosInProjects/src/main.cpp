#include <Arduino.h>
#include "MyMacros.h"
#include <Servo.h>

/* The servo pins go like this: RED=POWER ; BROWN=GROUND ; ORANGE=CONTROL */

#define _PHOTO_PIN A5
#define _SERVO_PIN PIN3

#define _MAX_SERVO_ANGLE 180.
#define _MAX_PHOTO_VALUE 1024.

float servoPhotoRatio = _MAX_SERVO_ANGLE / _MAX_PHOTO_VALUE;
Servo servoMotor;



void setup() {
  pinMode(_PHOTO_PIN, INPUT);

  Serial.begin(9600);
  while (!Serial) {};
  servoMotor.attach(_SERVO_PIN); 
}

#define _LOOP_PERIOD 1000
void loop() {
  int photoValue = analogRead(_PHOTO_PIN);

  debugPrintArgs("photoValue: ", static_cast<float>(photoValue));

  float servoAngle = static_cast<float>(photoValue) * servoPhotoRatio;
  debugPrintArgs("servoAngle: ", servoAngle);
  servoMotor.write(static_cast<int>(servoAngle));

  delay(_LOOP_PERIOD);
}

