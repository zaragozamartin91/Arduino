#include <Arduino.h>

#define __PNTLN(X) Serial.println(X)
#define __PNT(X) Serial.print(X)
#define __PNTVAL(X,Y) __PNT(X);__PNTLN(Y)

// #define __MONITOR_SPEED__ 38400
#define __MONITOR_SPEED__ 9600

#define __PING_EXAMPLE__ 1
#define __DO_NOTHING__ 2
#define __READ_END_SWITCH__ 5
#define __CONTROL_MOTOR__ 6
#define __RUN_MOTOR_UNTIL_END_SWITCH__ 7

#define __PROGRAM_TYPE__ __RUN_MOTOR_UNTIL_END_SWITCH__

/* ================================================================================================ */

#if __PROGRAM_TYPE__ == __PING_EXAMPLE__

int built_in_pin = 13;
int counter = 0;

void setup()
{
  // put your setup code here, to run once:
  pinMode(built_in_pin, OUTPUT); // pin 13 is connected to board led
  Serial.begin(__MONITOR_SPEED__);
  while (!Serial) {}; 
  Serial.println("Serial com BEGUN!");
}

void loop()
{
  
  counter++;

  Serial.print("Hello there #");Serial.println(counter);
  delay(2000);
  digitalWrite(built_in_pin, LOW);
  delay(2000);
  digitalWrite(built_in_pin, HIGH);
}


#elif __PROGRAM_TYPE__ == __DO_NOTHING__

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(__MONITOR_SPEED__);
  while (!Serial) {}; 
  Serial.println("__DO_NOTHING__ BEGUN!");
}

void loop() {
  delay(2000);
}


#elif __PROGRAM_TYPE__ == __READ_END_SWITCH__

#define __READ_PIN__ A5
#define __LED_PIN__ 12
#define __LOOP_DELAY__ 250
#define __LED_THRESHOLD__ 200

void setup() {
  // put your setup code here, to run once:
  pinMode(__READ_PIN__, INPUT);
  pinMode(__LED_PIN__, OUTPUT);

  Serial.begin(__MONITOR_SPEED__);
  while (!Serial) {}; 
  Serial.println("End switch draft ready to go");
}

int prevRead = 0;
long readCount = 0;
void loop() {
  int readValue = analogRead(__READ_PIN__);

  if (readValue > __LED_THRESHOLD__) { 
    prevRead = readValue;
    Serial.print("Value#");Serial.print(++readCount);Serial.print(": ");Serial.println(readValue);
    digitalWrite(__LED_PIN__, HIGH);
  } else {
    digitalWrite(__LED_PIN__, LOW);
  }
  delay(__LOOP_DELAY__);
}

#elif __PROGRAM_TYPE__ == __CONTROL_MOTOR__

#include <L298N.h>
#include <L298NX2.h>


// Pin definition
const unsigned int EN = 3;
const unsigned int IN1 = 4;
const unsigned int IN2 = 5;

// Create one motor instance
L298N motor(EN, IN1, IN2);

void printSomeInfo();

#define _MOTOR_MAX_SPEED_ 255
unsigned short motorSpeed = 0;

void setup() {
  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}
  Serial.println("Control single motor draft ready to go");

  motor.setSpeed(motorSpeed);
}

/* The min supply is 110 */
#define _MOTOR_SPEED_INCREMENT_ 10
#define _LOOP_DELAY_ 3000 

// void loop() {
//   Serial.print("Motor speed = ");Serial.println(motorSpeed);
//   motor.setSpeed(motorSpeed);

//   /* For some reason the motor can only run in one direction... */
//   motor.backward();

//   delay(_LOOP_DELAY_ / 2);
//   motor.stop();
//   delay(_LOOP_DELAY_ / 2);
//   motorSpeed = (motorSpeed + _MOTOR_SPEED_INCREMENT_) % _MOTOR_MAX_SPEED_;
// }

void loop() {
  unsigned short netMotorSpeed = _MOTOR_MAX_SPEED_ - motorSpeed;
  Serial.print("netMotorSpeed = ");Serial.println(netMotorSpeed);
  Serial.print("motorSpeed = ");Serial.println(netMotorSpeed);
  motor.setSpeed(netMotorSpeed);

  /* For some reason the motor can only run in one direction... */
  motor.backward();

  delay(_LOOP_DELAY_);
  motorSpeed = (motorSpeed + _MOTOR_SPEED_INCREMENT_) % _MOTOR_MAX_SPEED_;
}

/*
Print some informations in Serial Monitor
*/
void printSomeInfo() {
  Serial.print("Motor is moving = ");
  Serial.print(motor.isMoving());
  Serial.print(" at speed = ");
  Serial.println(motor.getSpeed());
}


#elif __PROGRAM_TYPE__ == __RUN_MOTOR_UNTIL_END_SWITCH__


#include <L298N.h>
#include <L298NX2.h>


#define __READ_PIN__ A5
#define __LED_PIN__ 12
#define __LOOP_DELAY__ 250
#define __LED_THRESHOLD__ 200


// Pin definition
#define _EN 3
#define _IN1 4
#define _IN2 5

// Create one motor instance
L298N motor(_EN, _IN1, _IN2);


#define _MOTOR_MAX_SPEED_ 255
unsigned short motorSpeed = 140;

void setup() {
  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}
  Serial.println("__RUN_MOTOR_UNTIL_END_SWITCH__ ready to go");

  // put your setup code here, to run once:
  pinMode(__READ_PIN__, INPUT);
  pinMode(__LED_PIN__, OUTPUT);


  motor.setSpeed(motorSpeed);
}


#define _LOOP_DELAY_ 3000 

unsigned int endSwitchCycles = 0;
unsigned int endSwitchValueReadCount = 0;
unsigned int loopCycles = 1;
void loop() {
  if (loopCycles > 5) {
    delay(_LOOP_DELAY_);
    return;
  }

  __PNT("motorSpeed = ");__PNTLN(motorSpeed);

  int endSwitchValue = analogRead(__READ_PIN__);
  if (endSwitchValue > __LED_THRESHOLD__) { 
    __PNT("Value#");__PNT(++endSwitchValueReadCount);__PNT(": ");__PNTLN(endSwitchValue);
    ++endSwitchCycles;
    digitalWrite(__LED_PIN__, HIGH);
  } else {
    digitalWrite(__LED_PIN__, LOW);
  }
  
  
  /* For some reason the motor can only run in one direction... */
  motor.backward();
  // accelerate(&motor, 0, motorSpeed, 500);
  delay(_LOOP_DELAY_);
  motor.stop();
  delay(_LOOP_DELAY_);
  
  ++loopCycles;
}

void accelerate(
  L298N* mtr,
  unsigned short startSpeed,
  unsigned short finalSpeed,
  unsigned long timeMs,
  int partition = 10) {

  div_t timeDiv = div(timeMs, partition);
  div_t speedDiv = div(finalSpeed, partition);

  mtr->setSpeed(startSpeed);
  for (int i = 0 ; i < partition && timeDiv.quot > 0 ; i++) {
    delay(timeDiv.quot);
    mtr->setSpeed( mtr->getSpeed() + speedDiv.quot );
  }

  if (timeDiv.rem > 0) {
    delay(timeDiv.rem);
    mtr->setSpeed(finalSpeed);
  }
}


#endif