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
#include <L298NX2.h>


#define _PROGRAM_DISABLED 0
#define _CONTROL_SINGLE_MOTOR 1
#define _CONTROL_DUAL_MOTORS 2
#define _IR_REMOTE_DEMO 3
#define _IR_REMOTE_DEMO_OLD 4
#define _CONTROL_MOTORS_WITH_REMOTE 5
#define _IR_REMOTE_DEMO_ELEGOO 6


#define _DC_MOTOR_PROGRAM_TYPE _CONTROL_DUAL_MOTORS

// ==================================================

void printSomeInfo();

const unsigned int STDBY_PIN = 13;

#if _DC_MOTOR_PROGRAM_TYPE == _CONTROL_SINGLE_MOTOR
// Pin definition
const unsigned int EN = 3;
const unsigned int IN1 = 4;
const unsigned int IN2 = 5;

// Create one motor instance
L298N motor(EN, IN1, IN2);

void setup() {
  pinMode(STDBY_PIN, OUTPUT);
  digitalWrite(STDBY_PIN, HIGH);

  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}

  // Set initial speed
  // NOTE: with 5v suppply, the motor cannot move at this speed
  motor.setSpeed(70); 
}

void loop() {
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

/*
Print some informations in Serial Monitor
*/
void printSomeInfo() {
  Serial.print("Motor is moving = ");
  Serial.print(motor.isMoving());
  Serial.print(" at speed = ");
  Serial.println(motor.getSpeed());
}

#elif _DC_MOTOR_PROGRAM_TYPE == _CONTROL_DUAL_MOTORS

// Pin definition
const unsigned int PWMA = 6;
const unsigned int _A12 = 7;
const unsigned int _A11 = 8;
 
const unsigned int _B11 = 9;
const unsigned int _B12 = 10;
const unsigned int PWMB = 11;

// Initialize both motors
L298NX2 motors(PWMA, _A11,_A12, PWMB, _B11, _B12);

void setup() {
  pinMode(STDBY_PIN, OUTPUT);
  digitalWrite(STDBY_PIN, HIGH);

  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}
}

void loop() {
  // Set initial speed for both motors
  motors.setSpeed(150);

  // Tell both motors to go forward (may depend by your wiring)
  motors.forward();

  // Alternative method:
  // motors.run(L298N::FORWARD);

  //print the motor status in the serial monitor
  printSomeInfo();

  delay(3000);

  // Stop
  motors.stop();

  // Alternative method:
  // motors.run(L298N::STOP);

  printSomeInfo();

  delay(3000);

  // Change individual speeds
  // motors.setSpeedA(255);
  // motors.setSpeedB(100);

  // // Tell motor A to go back (may depend by your wiring)
  // motors.backwardA();

  // // Alternative method:
  // // motors.runA(L298N::BACKWARD);

  // // Tell motor B to go forward (may depend by your wiring)
  // motors.backwardB();

  // // Alternative method:
  // // motors.runB(L298N::FORWARD);

  // printSomeInfo();

  // delay(3000);

  // // Stop
  // motors.stop();

  // printSomeInfo();

  // // Change individual speeds
  // motors.setSpeedA(100);
  // motors.setSpeedB(255);

  // delay(3000);
}

/*
Print some informations in Serial Monitor
*/
void printSomeInfo() {
  Serial.print("Motor A is moving = ");
  Serial.print(motors.isMovingA() ? "YES" : "NO");
  Serial.print(" at speed = ");
  Serial.println(motors.getSpeedA());
  Serial.print("Motor B is moving = ");
  Serial.print(motors.isMovingB() ? "YES" : "NO");
  Serial.print(" at speed = ");
  Serial.println(motors.getSpeedB());
}

#elif _DC_MOTOR_PROGRAM_TYPE == _IR_REMOTE_DEMO

#include "IRremote.hpp"

#define IR_RECEIVE_PIN 2

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    uint16_t command = IrReceiver.decodedIRData.command;
    Serial.println(command);
    delay(100);  // wait a bit
    IrReceiver.resume();
  }
}

void printSomeInfo() {
    Serial.println("IR remote demo");
}

#elif _DC_MOTOR_PROGRAM_TYPE == _IR_REMOTE_DEMO_OLD

#include <IRremote.h>

int recPin = 3;
IRrecv IR(recPin);
decode_results cmd;


void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  IR.enableIRIn();
}

void loop() {
  while (IR.decode(&cmd) == 0) {
  }

  Serial.println(cmd.value, HEX);
  Serial.println(cmd.overflow);
  Serial.println();

  delay(1000);
  IR.resume();
}


void printSomeInfo() {
    Serial.println("IR remote demo OLD");
}


#elif _DC_MOTOR_PROGRAM_TYPE == _CONTROL_MOTORS_WITH_REMOTE


#include <IRremote.h>

int recPin = 3;
IRrecv IR(recPin);
decode_results cmd;

// Pin definition
const unsigned int PWMA = 6;
const unsigned int _A12 = 7;
const unsigned int _A11 = 8;
 
const unsigned int _B11 = 9;
const unsigned int _B12 = 10;
const unsigned int PWMB = 11;

// Initialize both motors
L298NX2 motors(PWMA, _A12, _A11, PWMB, _B11, _B12);

void setup() {
  pinMode(STDBY_PIN, OUTPUT);
  digitalWrite(STDBY_PIN, HIGH);

  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}

  IR.enableIRIn();
}

void loop() {
  while (IR.decode(&cmd) == 0) {
  }

  Serial.println(cmd.value, HEX);
  Serial.println(cmd.overflow);
  Serial.println();

  delay(1000);
  IR.resume();
}


void printSomeInfo() {
    Serial.println("IR remote demo OLD");
}

_IR_REMOTE_DEMO_ELEGOO
#elif _DC_MOTOR_PROGRAM_TYPE == _IR_REMOTE_DEMO_ELEGOO

#include "IRremote.h"

int receiver = 2; // Signal Pin of IR receiver to Arduino Digital Pin 11

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

/*-----( Function )-----*/
void translateIR() {
  // takes action based on IR code received
  // describing Remote IR codes 

  switch(results.value) {
    case 0xFFA25D: Serial.println("POWER");         break;
    case 0xFFE21D: Serial.println("FUNC/STOP");     break;
    case 0xFF629D: Serial.println("VOL+");          break;
    case 0xFF22DD: Serial.println("FAST BACK");     break;
    case 0xFF02FD: Serial.println("PAUSE");         break;
    case 0xFFC23D: Serial.println("FAST FORWARD");  break;
    case 0xFFE01F: Serial.println("DOWN");          break;
    case 0xFFA857: Serial.println("VOL-");          break;
    case 0xFF906F: Serial.println("UP");            break;
    case 0xFF9867: Serial.println("EQ");            break;
    case 0xFFB04F: Serial.println("ST/REPT");       break;
    case 0xFF6897: Serial.println("0");             break;
    case 0xFF30CF: Serial.println("1");             break;
    case 0xFF18E7: Serial.println("2");             break;
    case 0xFF7A85: Serial.println("3");             break;
    case 0xFF10EF: Serial.println("4");             break;
    case 0xFF38C7: Serial.println("5");             break;
    case 0xFF5AA5: Serial.println("6");             break;
    case 0xFF42BD: Serial.println("7");             break;
    case 0xFF4AB5: Serial.println("8");             break;
    case 0xFF52AD: Serial.println("9");             break;
    case 0xFFFFFFFF: Serial.println(" REPEAT");     break;  

    default:
      
      Serial.print("other button: ");Serial.print(results.value, HEX);Serial.println();

  }// End Case

  delay(500); // Do not get immediate repeat
} //END translateIR

/*
tv remote:
- 20DF02FD = UP
- 20DF827D = DOWN
- 20DFE01F = LEFT
- 20DF609F = RIGHT
- 20DF22DD = OK/CENTER
- 20DF40BF = VOL_UP
- 20DFC03F = VOL_DOWN
*/

void setup() {
  Serial.begin(9600);
  while(!Serial) {};
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  // have we received an IR signal?
  if (irrecv.decode(&results)) {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
}/* --(end main loop )-- */


#else
void setup() {
  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}
}

void loop() {
  printSomeInfo();
  delay(1000);
}


/*
Print some informations in Serial Monitor
*/
void printSomeInfo() {
    Serial.println("Program disabled");
}

#endif


