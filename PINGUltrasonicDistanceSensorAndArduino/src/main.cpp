#include <Arduino.h>

// From video https://www.youtube.com/watch?v=zACmjwvbils

/*
  Ping))) Sensor

  This sketch reads a PING))) ultrasonic rangefinder and returns the distance
  to the closest object in range. To do this, it sends a pulse to the sensor to
  initiate a reading, then listens for a pulse to return. The length of the
  returning pulse is proportional to the distance of the object from the sensor.

  The circuit:
	- +V connection of the PING))) attached to +5V
	- GND connection of the PING))) attached to ground
	- SIG connection of the PING))) attached to digital pin 7

  created 3 Nov 2008
  by David A. Mellis
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Ping
*/

// this constant won't change. It's the pin number of the sensor's output:
#define SENSOR_PING_PIN 6

#define LED_PIN_1 9
#define LED_PIN_2 11
#define LED_PIN_3 13

void setup() {
  // initialize serial communication:
  Serial.begin(9600);

  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);  
  pinMode(LED_PIN_3, OUTPUT);
}

#define _LOOP_DELAY 250
void loop() {
  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  long duration, inches, cm;

  // The sensor pin is used for both , sending a pulse and reading a pulse
  // That is why it has to be set to output and input in the loop function

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(SENSOR_PING_PIN, OUTPUT);
  digitalWrite(SENSOR_PING_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(SENSOR_PING_PIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(SENSOR_PING_PIN, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(SENSOR_PING_PIN, INPUT);
  duration = pulseIn(SENSOR_PING_PIN, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();

  bool led1On = cm < 150;
  bool led2On = cm < 100;
  bool led3On = cm < 50;

  digitalWrite(LED_PIN_1, led1On);
  digitalWrite(LED_PIN_2, led2On);
  digitalWrite(LED_PIN_3, led3On);

  delay(_LOOP_DELAY);
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: https://www.parallax.com/package/ping-ultrasonic-distance-sensor-downloads/
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}
