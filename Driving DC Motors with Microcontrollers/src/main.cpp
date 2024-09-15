/*
Author  : Andrea Lombardo
Site    : https://www.lombardoandrea.com
Source  : https://github.com/AndreaLombardo/L298N/

Here you can see how to work in a common configuration. 

Speed range go from 0 to 255, default is 100.
Use setSpeed(speed) to change.

Sometimes at lower speed motors seems not running.
It's normal, may depends by motor and power supply.

Wiring schema in file "L298N - Schema_with_EN_pin.png"
*/

// Include the library
#include <L298N.h>

#define _CONTROL_SINGLE_MOTOR 1
#define _CONTROL_DUAL_MOTOR 2
#define _PROGRAM_DISABLED 0

#define _DC_MOTOR_PROGRAM_TYPE _PROGRAM_DISABLED


// Pin definition
#define _STDBY_PIN 13
const unsigned int EN = 10;
const unsigned int IN1 = 9;
const unsigned int IN2 = 8;

// Create one motor instance
L298N motor(EN, IN1, IN2);

void printSomeInfo();

#if _DC_MOTOR_PROGRAM_TYPE == _CONTROL_SINGLE_MOTOR
void setup()
{
  pinMode(_STDBY_PIN, OUTPUT);

  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial)
  {
    //do nothing
  }

  digitalWrite(_STDBY_PIN, HIGH);

  // Set initial speed
  motor.setSpeed(70);
}

void loop()
{

  // Tell the motor to go forward (may depend by your wiring)
  motor.forward();

  // Alternative method:
  // motor.run(L298N::FORWARD);

  //print the motor status in the serial monitor
  printSomeInfo();

  delay(3000);

  // Stop
  motor.stop();

  // Alternative method:
  // motor.run(L298N::STOP);

  printSomeInfo();

  // Change speed
  motor.setSpeed(255);

  delay(3000);

  // Tell the motor to go back (may depend by your wiring)
  motor.backward();

  // Alternative method:
  // motor.run(L298N::BACKWARD);

  printSomeInfo();

  motor.setSpeed(120);

  delay(3000);

  // Stop
  motor.stop();

  printSomeInfo();

  delay(3000);
}
#else
void setup() {
  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}
}

void loop() {
  Serial.println("Program disabled");
  delay(1000);
}
#endif


/*
Print some informations in Serial Monitor
*/
void printSomeInfo()
{
  Serial.print("Motor is moving = ");
  Serial.print(motor.isMoving());
  Serial.print(" at speed = ");
  Serial.println(motor.getSpeed());
}
