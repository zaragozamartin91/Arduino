#include <Arduino.h>
#include <Servo.h>
#include "MyMacros.h"

/* From video https://www.youtube.com/watch?v=B6YEQj4d5WU&list=PLGs0VKk2DiYw-L-RibttcvK-WBZm8WLEP&index=32 */
/* And video https://www.youtube.com/watch?v=ChHNI8yt69g&list=PLGs0VKk2DiYw-L-RibttcvK-WBZm8WLEP&index=35 */

/* The servo pins go like this: RED=POWER ; BROWN=GROUND ; ORANGE=CONTROL */

#define _X_PIN A1
#define _Y_PIN A5
#define _SWITCH_PIN 11
#define _SERVO_PIN PIN3

#define _MAX_SERVO_ANGLE 160
#define _MAX_JOYSTICK_READ 1023
#define _SERVO_SPPEED_RATIO 50
#define _LOOP_PERIOD 100
#define _RESET_BUTTON_PIN 12


Servo servoMotor;
int servoPosition = 0;
float fServoSpeed = static_cast<float>(_SERVO_SPPEED_RATIO) * static_cast<float>(_LOOP_PERIOD) / 1000.;
int iServoSpeed = static_cast<int>(fServoSpeed);

void printJoystickInput(int switchRead, int xRead, int yRead);

int mapJoystickToServo(int jRead);

void moveServo(int direction);

void resetServoPosition();

void setup() {
  pinMode(_X_PIN, INPUT);
  pinMode(_Y_PIN, INPUT);
  pinMode(_SWITCH_PIN, INPUT);

  /* Switch in joystick works like a pull up resistor ; sending a high signal to make reads more intuitive */
  // By doing this ; if switch is pressed, a LOW value will be read
  digitalWrite(_SWITCH_PIN, HIGH);

  pinMode(_SERVO_PIN, OUTPUT);

  Serial.begin(9600);
  while(!Serial) {}

  // start servo w/ 0 angle
  servoMotor.attach(_SERVO_PIN);
  servoMotor.write(0);

  /* By writing a high voltage into the pin and also reading from it, the button behaves like a pull up resistor button.
  This means that it will read 0 when pressed and 1 when not pressed */
  pinMode(_RESET_BUTTON_PIN, INPUT);
  digitalWrite(_RESET_BUTTON_PIN, HIGH);
}

void loop() {
  auto xRead = analogRead(_X_PIN);
  auto yRead = analogRead(_Y_PIN);
  auto switchRead = digitalRead(_SWITCH_PIN);

  printJoystickInput(switchRead, xRead, yRead);

  int servoDirection = 
    yRead > 700 ? -1 
    : yRead < 300 ? 1 
    : 0;

  moveServo(servoDirection);

  int resetButtonRead = digitalRead(_RESET_BUTTON_PIN);
  if (resetButtonRead == 0) resetServoPosition(); // reset servo position on button press

  delay(_LOOP_PERIOD);
}


void printJoystickInput(int switchRead, int xRead, int yRead) {
  Serial.print("switchRead=");
  Serial.print(switchRead);
  Serial.print(" ; xRead=");
  Serial.print(xRead);
  Serial.print(" ; yRead=");
  Serial.println(yRead);
}

int mapJoystickToServo(int jRead) {
  float jReadF = static_cast<float>(jRead);
  float maxServoAngle = static_cast<float>(_MAX_SERVO_ANGLE);
  float maxJoystickRead = static_cast<float>(_MAX_JOYSTICK_READ);

  float servoAngle = jReadF * maxServoAngle / maxJoystickRead;
  return static_cast<int>(servoAngle);
}

void moveServo(int direction) {
  int moveIncrement = iServoSpeed * direction;
  int newServoPosition = servoPosition + moveIncrement;
  servoPosition = max(0, newServoPosition);
  servoPosition = min(_MAX_SERVO_ANGLE, newServoPosition);
  servoMotor.write(servoPosition);
}

void resetServoPosition() {
  servoPosition = 0;
}