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
#define __CONTROL_MOTOR_WITH_PUSH_BUTTON__ 8
#define __CONTROL_MOTOR_WITH_BUTTON_AND_CHECK_END_SWITCH__ 9
#define __STEPPER_ACCELERATOR_POC__ 10
#define __STEPPER_DECCELERATOR_POC__ 11
#define __POTENTIOMETER_POC__ 12
#define __PUSH_BUTTON_POC__ 13
#define __LCD_POC__ 14
#define __MOTOR_W_LCD_POC__ 15
#define __TIMER_W_MOTOR_N_LCD_POC__ 16
#define __TIMER_W_MOTOR_N_LCD_W_STATE_MACHINE_POC__ 17

#define __PROGRAM_TYPE__ __TIMER_W_MOTOR_N_LCD_W_STATE_MACHINE_POC__

// DC motor link:https://www.amazon.se/dp/B096Z5C3R4/ref=sspa_dk_detail_1?psc=1&sp_csd=d2lkZ2V0TmFtZT1zcF9kZXRhaWw

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


#elif __PROGRAM_TYPE__ == __CONTROL_MOTOR_WITH_PUSH_BUTTON__


#include <L298N.h>
#include <L298NX2.h>


#define __LOOP_DELAY__ 250
#define __CONTROL_MOTOR_PIN__ 13


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
  Serial.println("__CONTROL_MOTOR_WITH_PUSH_BUTTON__ ready to go");

  /* Shortcut of pinMode(X,INPUT);digitalWrite(X,HIGH); */
  pinMode(__CONTROL_MOTOR_PIN__, INPUT_PULLUP);

}


void loop() {
  bool motorControlValue = digitalRead(__CONTROL_MOTOR_PIN__);

  bool buttonPressed = !motorControlValue;
  if (buttonPressed) __PNTLN("Button pressed!");
  

  bool moveMotor = buttonPressed;
  if (moveMotor) {
    motor.setSpeed(motorSpeed);
    motor.backward();
  } else {
    motor.setSpeed(0);
    motor.stop();
  }

  delay(__LOOP_DELAY__);
}



#elif __PROGRAM_TYPE__ == __CONTROL_MOTOR_WITH_BUTTON_AND_CHECK_END_SWITCH__


#include <L298N.h>
#include <L298NX2.h>

#define __READ_PIN__ A5
#define __LED_PIN__ 12
#define __LOOP_DELAY__ 50
#define __CONTROL_MOTOR_PIN__ 13
#define __LED_THRESHOLD__ 200


// Pin definition
#define _EN 3
#define _IN1 4
#define _IN2 5

// Create one motor instance
L298N motor(_EN, _IN1, _IN2);


#define _MOTOR_MAX_SPEED_ 255
unsigned short motorSpeed = 125;

void setup() {
  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}
  Serial.println("__CONTROL_MOTOR_WITH_PUSH_BUTTON__ ready to go");

  /* Shortcut of pinMode(X,INPUT);digitalWrite(X,HIGH); */
  pinMode(__CONTROL_MOTOR_PIN__, INPUT_PULLUP);
  pinMode(__READ_PIN__, INPUT);
  pinMode(__LED_PIN__, OUTPUT);
}

unsigned int endSwitchValueReadCount = 0;
void loop() {
  int endSwitchValue = analogRead(__READ_PIN__);
  bool motorControlValue = digitalRead(__CONTROL_MOTOR_PIN__);

  bool buttonPressed = !motorControlValue;
  if (buttonPressed) {
    __PNTVAL("Value#",++endSwitchValueReadCount);
    __PNTVAL("endSwitchValue: ", endSwitchValue);
  };
  
  bool moveMotor = buttonPressed;
  if (moveMotor) {
    motor.setSpeed(motorSpeed);
    motor.backward();
  } else {
    motor.setSpeed(0);
    motor.stop();
  }

  if (endSwitchValue > __LED_THRESHOLD__) { 
    digitalWrite(__LED_PIN__, HIGH);
  } else {
    digitalWrite(__LED_PIN__, LOW);
  }

  delay(__LOOP_DELAY__);
}


#elif __PROGRAM_TYPE__ == __STEPPER_ACCELERATOR_POC__


#include <L298N.h>
#include <L298NX2.h>
#include "Stepper.h"
#include <limits.h>

#define __READ_PIN__ A5
#define __LED_PIN__ 12
#define __LOOP_DELAY__ 50
#define __CONTROL_MOTOR_PIN__ 13
#define __LED_THRESHOLD__ 200


// Pin definition
#define _EN 3
#define _IN1 4
#define _IN2 5

// Create one motor instance
L298N motor(_EN, _IN1, _IN2);


#define __MAX_MOTOR_SPEED__ 125
#define __ACCELERATION__ 5
void accelerate();
mz::Stepper accelerator(&millis, &accelerate, 100L, UINT_MAX);
unsigned short motorSpeed = 0;

void setup() {
  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}
  Serial.println("__STEPPER_ACCELERATOR_POC__ ready to go");

  /* Shortcut of pinMode(X,INPUT);digitalWrite(X,HIGH); */
  pinMode(__CONTROL_MOTOR_PIN__, INPUT_PULLUP);
  pinMode(__READ_PIN__, INPUT);
  pinMode(__LED_PIN__, OUTPUT);

  accelerator.setup();
}

unsigned int endSwitchValueReadCount = 0;
void loop() {
  int endSwitchValue = analogRead(__READ_PIN__);
  bool motorControlValue = digitalRead(__CONTROL_MOTOR_PIN__);


  bool buttonPressed = !motorControlValue;
  if (buttonPressed) {
    __PNTVAL("Value#",++endSwitchValueReadCount);
    // __PNTVAL("endSwitchValue: ", endSwitchValue);
    __PNTVAL("motorSpeed: ", motorSpeed);
  }
  auto ledValue = endSwitchValue > __LED_THRESHOLD__ ? HIGH : LOW;
  digitalWrite(__LED_PIN__, ledValue);


  bool moveMotor = buttonPressed;

  if (moveMotor && motor.isMoving()) {
    // keep on moving
    accelerator.loop();
    motor.setSpeed(motorSpeed);
    motor.backward();
    __PNTLN("Looping accelerator");
  } else if(moveMotor && !motor.isMoving()) {
    // start moving
    accelerator.setup();
    motor.backward();
    __PNTLN("Moving motor");
  } else if(!moveMotor) {
    // halt
    motor.setSpeed(0);
    motor.stop();
    motorSpeed = 0;
    // __PNTLN("Stopping motor");
  }


  delay(__LOOP_DELAY__);
}


void accelerate() {
  unsigned short newSpeed = min(motorSpeed + __ACCELERATION__, __MAX_MOTOR_SPEED__);
  motorSpeed = newSpeed;
}


#elif __PROGRAM_TYPE__ == __STEPPER_DECCELERATOR_POC__


#include <L298N.h>
#include <L298NX2.h>
#include "Stepper.h"
#include <limits.h>

#define __END_SWITCH_PIN__ A5
#define __LED_PIN__ A3
#define __LOOP_DELAY__ 5
#define __CONTROL_MOTOR_PIN__ A4
#define __LED_THRESHOLD__ 200


// Motor Pin definition
// lcd uses LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define _EN 3 // SQUIGLY
#define _IN1 4
#define _IN2 5 // SQUIGLY

// Create one motor instance
L298N motor(_EN, _IN1, _IN2);


#define __MAX_MOTOR_SPEED__ 150
#define __MIN_MOTOR_SPEED__ 70
#define __ACCELERATION__ 4
unsigned short motorSpeed = __MAX_MOTOR_SPEED__;
void deccelerate();
mz::Stepper accelerator(&millis, &deccelerate, 25L, UINT_MAX);

void setup() {
  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}
  Serial.println("__STEPPER_DECCELERATOR_POC__ ready to go");

  /* Shortcut of pinMode(X,INPUT);digitalWrite(X,HIGH); */
  pinMode(__CONTROL_MOTOR_PIN__, INPUT);
  pinMode(__END_SWITCH_PIN__, INPUT);
  pinMode(__LED_PIN__, OUTPUT);

  accelerator.setup();
}

unsigned int endSwitchValueReadCount = 0;
void loop() {
  int endSwitchValue = analogRead(__END_SWITCH_PIN__);
  bool buttonPressed = digitalRead(__CONTROL_MOTOR_PIN__);

  if (buttonPressed) {
    __PNTVAL("Value#",++endSwitchValueReadCount);
    __PNTVAL("endSwitchValue: ", endSwitchValue);
    __PNTVAL("motorSpeed: ", motorSpeed);
  }
  auto ledValue = endSwitchValue > __LED_THRESHOLD__ ? HIGH : LOW;
  digitalWrite(__LED_PIN__, ledValue);


  bool moveMotor = buttonPressed;

  if (moveMotor && motor.isMoving()) {
    // keep on moving
    accelerator.loop();
    motor.setSpeed(motorSpeed);
    motor.backward();
    __PNTLN("Looping accelerator");
  } else if(moveMotor && !motor.isMoving()) {
    // start moving
    motorSpeed = __MAX_MOTOR_SPEED__; // reset motor speed
    accelerator.setup();
    motor.backward();
    __PNTLN("Moving motor");
  } else if(!moveMotor) {
    // halt
    motor.setSpeed(0);
    motor.stop();
    motorSpeed = 0;
    // __PNTLN("Stopping motor");
  }


  delay(__LOOP_DELAY__);
}


void deccelerate() {
  unsigned short newSpeed = max(motorSpeed - __ACCELERATION__, __MIN_MOTOR_SPEED__);
  motorSpeed = newSpeed;
}


#elif __PROGRAM_TYPE__ == __POTENTIOMETER_POC__


#include <limits.h>

#define __POTENTIOMETER_PIN__ A0
#define __LED_PIN__ 12
#define __LOOP_DELAY__ 3000


void setup() {
  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}
  Serial.println("__POTENTIOMETER_POC__ ready to go");

  /* Shortcut of pinMode(X,INPUT);digitalWrite(X,HIGH); */
  pinMode(__POTENTIOMETER_PIN__, INPUT);
  pinMode(__LED_PIN__, OUTPUT);

}

void loop() {
  int potentiometerValue = analogRead(__POTENTIOMETER_PIN__);
  __PNTVAL("potentiometerValue: ", potentiometerValue);

  boolean ledPinHigh = potentiometerValue > 500;
  digitalWrite(__LED_PIN__, ledPinHigh);

  delay(__LOOP_DELAY__);
}

#elif __PROGRAM_TYPE__ == __PUSH_BUTTON_POC__


#include <limits.h>

#define __LED_PIN__ 7
#define __LOOP_DELAY__ 1000
#define __BUTTON_PIN__ A4


void setup() {
  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}
  Serial.println("__PUSH_BUTTON_POC__ ready to go");

  pinMode(__BUTTON_PIN__, INPUT);
  pinMode(__LED_PIN__, OUTPUT);
}

unsigned int endSwitchValueReadCount = 0;
void loop() {
  // should this be an analog read?
  bool buttonValue = digitalRead(__BUTTON_PIN__);
  digitalWrite(__LED_PIN__, buttonValue);

  __PNTVAL("buttonValue: ", buttonValue);

  delay(__LOOP_DELAY__);
}


#elif __PROGRAM_TYPE__ == __LCD_POC__

#include <LiquidCrystal.h>

/* Definition of LCD pins */
/* Problem with pin_11 and the motor module */
/* Avoid using pin_11 */
#define __RS_PIN__ 6
#define __EN_PIN__ 7
#define __D4_PIN__ 8
#define __D5_PIN__ 9
#define __D6_PIN__ 12
#define __D7_PIN__ 13

// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(__RS_PIN__, __EN_PIN__, __D4_PIN__, __D5_PIN__, __D6_PIN__, __D7_PIN__);

#define __LOOP_DELAY__ 500

#define __SCROLL_LEN__ 20

void setup() {
  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}
  Serial.println("__LCD_POC__ ready to go");

  /* Shortcut of pinMode(X,INPUT);digitalWrite(X,HIGH); */
  // pinMode(__CONTROL_MOTOR_PIN__, INPUT);
  // pinMode(__END_SWITCH_PIN__, INPUT);
  // pinMode(__LED_PIN__, OUTPUT);

    // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Clears the LCD screen
  lcd.clear();
  lcd.setCursor(0, 0);
}

int scrollIteration = 0;
void loop() {

  if (scrollIteration > __SCROLL_LEN__) {
    // reset 
    scrollIteration = 0;
    // Print a message to the LCD.
    lcd.print("Scrolling Text Demo");
    delay(__LOOP_DELAY__ * 2);
  } else {
    ++scrollIteration;
    lcd.scrollDisplayLeft();  // scroll everything to the left by one position
    delay(__LOOP_DELAY__);
  }
}


#elif __PROGRAM_TYPE__ == __MOTOR_W_LCD_POC__


#include <L298N.h>
#include <L298NX2.h>
#include "Stepper.h"
#include <limits.h>
#include <LiquidCrystal.h>
#include "MotorControlState.h"


// Motor Pin definition
// Create one motor instance
#define __MOTOR_ENABLE_PIN__ 3
#define __MOTOR_INI1_PIN__ 4
#define __MOTOR_INI2_PIN__ 5
L298N motor(__MOTOR_ENABLE_PIN__, __MOTOR_INI1_PIN__, __MOTOR_INI2_PIN__);

#define __MAX_MOTOR_SPEED_SWITCH_OFF__ 150 // initial motor speed when end switch is NOT pressed
#define __MAX_MOTOR_SPEED_SWITCH_ON__ 190 // initial motor speed when end switch IS pressed
#define __MIN_MOTOR_SPEED__ 80
#define __ACCELERATION__ 5
unsigned short motorSpeed = __MAX_MOTOR_SPEED_SWITCH_ON__;
void deccelerate();
mz::Stepper accelerator(&millis, &deccelerate, 25L, UINT_MAX);

/* Definition of LCD pins */
/* Problem with pin_11 and the motor module */
/* Avoid using pin_11 */
// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
#define __RS_PIN__ 6
#define __EN_PIN__ 7
#define __D4_PIN__ 8
#define __D5_PIN__ 9
#define __D6_PIN__ 12
#define __D7_PIN__ 13
LiquidCrystal lcd(__RS_PIN__, __EN_PIN__, __D4_PIN__, __D5_PIN__, __D6_PIN__, __D7_PIN__);


#define __END_SWITCH_PIN__ A5
#define __LED_PIN__ A3
#define __CONTROL_MOTOR_PIN__ A4
#define __LED_THRESHOLD__ 50

void resetLcd(LiquidCrystal& lcd);

mz::MotorControlState parseMotorControlState(bool moveMotor, bool motorIsMoving);

void setup() {
  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}
  Serial.println("__MOTOR_W_LCD_POC__ ready to go");

  /* Shortcut of pinMode(X,INPUT);digitalWrite(X,HIGH); */
  pinMode(__CONTROL_MOTOR_PIN__, INPUT);
  pinMode(__END_SWITCH_PIN__, INPUT);
  pinMode(__LED_PIN__, OUTPUT);

  accelerator.setup();

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Clears the LCD screen
  resetLcd(lcd);
  lcd.setCursor(0, 0);
  lcd.print("Waiting for");
  lcd.setCursor(0, 1);
  lcd.print("input...");
}

#define __LOOP_DELAY_MS__ 5

unsigned int endSwitchValueReadCount = 0;
void loop() {
  int endSwitchValue = analogRead(__END_SWITCH_PIN__);
  bool buttonPressed = digitalRead(__CONTROL_MOTOR_PIN__);

  if (buttonPressed) {
    __PNTVAL("Value#",++endSwitchValueReadCount);
    __PNTVAL("endSwitchValue: ", endSwitchValue);
    __PNTVAL("motorSpeed: ", motorSpeed);
  }
  bool endSwitchPressed = endSwitchValue > __LED_THRESHOLD__; 
  digitalWrite(__LED_PIN__, endSwitchPressed);

  bool moveMotor = buttonPressed;

  auto motorControlState = parseMotorControlState(moveMotor, motor.isMoving());

  switch (motorControlState) {
    case mz::MotorControlState::MOTOR_CONTROL_MOVING:
      // keep on moving
      accelerator.loop();
      motor.setSpeed(motorSpeed);
      motor.backward();
      lcd.setCursor(0, 0);
      lcd.print("Moving motor");
      __PNTLN("Looping accelerator");
      break;
    
    case mz::MotorControlState::MOTOR_CONTROL_HALTING:
      // halt
      motor.setSpeed(0);
      motor.stop();
      motorSpeed = 0;
      resetLcd(lcd);
      break;

    case mz::MotorControlState::MOTOR_CONTROL_STARTING:
      // start moving

      // reset motor speed
      motorSpeed = endSwitchPressed 
        ? __MAX_MOTOR_SPEED_SWITCH_ON__ 
        : __MAX_MOTOR_SPEED_SWITCH_OFF__; 
      accelerator.setup();
      motor.backward();
      resetLcd(lcd);
      __PNTLN("Moving motor");
      break;

    case mz::MotorControlState::MOTOR_CONTROL_IDLE:
      lcd.setCursor(0, 0);
      lcd.print("Waiting for");
      lcd.setCursor(0, 1);
      lcd.print("input...");
      break;

  
  default:
    break;
  }

  delay(__LOOP_DELAY_MS__);
}


void deccelerate() {
  unsigned short newSpeed = max(motorSpeed - __ACCELERATION__, __MIN_MOTOR_SPEED__);
  motorSpeed = newSpeed;
}

void resetLcd(LiquidCrystal& lcd) {
  lcd.clear();
  lcd.setCursor(0, 0);
}

mz::MotorControlState parseMotorControlState(bool moveMotor, bool motorIsMoving) {
    if (moveMotor && motorIsMoving)   return mz::MotorControlState::MOTOR_CONTROL_MOVING;
    if (!moveMotor && motorIsMoving)  return mz::MotorControlState::MOTOR_CONTROL_HALTING;
    if (moveMotor && !motorIsMoving)  return mz::MotorControlState::MOTOR_CONTROL_STARTING;
    /* else */                        return mz::MotorControlState::MOTOR_CONTROL_IDLE;
}



#elif __PROGRAM_TYPE__ == __TIMER_W_MOTOR_N_LCD_POC__

/* Implementing it this way seems too complicated */

/* On button press start a countdown of 5 seconds */
/* Display a message on the LCD screen with the countdown */
/* After 5 seconds, move the motor */
/* When the end-switch signal triggers, stop the motor */

#include <L298N.h>
#include "Stepper.h"
#include <limits.h>
#include <LiquidCrystal.h>
#include "MotorControlState.h"

#define __LOOP_DELAY_MS__ 5


// Motor Pin definition
// Create one motor instance
#define __MOTOR_ENABLE_PIN__ 3
#define __MOTOR_INI1_PIN__ 4
#define __MOTOR_INI2_PIN__ 5
L298N motor(__MOTOR_ENABLE_PIN__, __MOTOR_INI1_PIN__, __MOTOR_INI2_PIN__);

#define __MAX_MOTOR_SPEED_SWITCH_OFF__ 150 // initial motor speed when end switch is NOT pressed
#define __MAX_MOTOR_SPEED_SWITCH_ON__ 190 // initial motor speed when end switch IS pressed
#define __MIN_MOTOR_SPEED__ 80
#define __ACCELERATION__ 5
unsigned short motorSpeed = __MAX_MOTOR_SPEED_SWITCH_ON__;
void deccelerate();
mz::Stepper accelerator(&millis, &deccelerate, 25L, UINT_MAX);

/* Definition of LCD pins */
/* Problem with pin_11 and the motor module */
/* Avoid using pin_11 */
// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
#define __RS_PIN__ 6
#define __EN_PIN__ 7
#define __D4_PIN__ 8
#define __D5_PIN__ 9
#define __D6_PIN__ 12
#define __D7_PIN__ 13
LiquidCrystal lcd(__RS_PIN__, __EN_PIN__, __D4_PIN__, __D5_PIN__, __D6_PIN__, __D7_PIN__);


#define __END_SWITCH_PIN__ A5
#define __LED_PIN__ A3
#define __CONTROL_MOTOR_PIN__ A4
#define __LED_THRESHOLD__ 50

void resetLcd(LiquidCrystal& lcd);

mz::MotorControlState parseMotorControlState(bool moveMotor, bool motorIsMoving);

void haltMotor();
void startMotor();

mz::Stepper noOpStepper(
  &millis,
  [](){},
  __LOOP_DELAY_MS__,
  0
);

mz::Stepper startMotorTimer(
  &millis,
  &startMotor,
  5000,
  1
);

mz::Stepper stopMotorTimer(
  &millis,
  &haltMotor,
  200,
  1
);

mz::Stepper NO_OP_STEPPER(
    [](){ unsigned long res = 0; return res; },
    [](){ },
    0,
    0
);

mz::Stepper* currentStepper = &NO_OP_STEPPER;

void setup() {
  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}
  Serial.println("__TIMER_W_MOTOR_N_LCD_POC__ ready to go");

  /* Shortcut of pinMode(X,INPUT);digitalWrite(X,HIGH); */
  pinMode(__CONTROL_MOTOR_PIN__, INPUT);
  pinMode(__END_SWITCH_PIN__, INPUT);
  pinMode(__LED_PIN__, OUTPUT);

  accelerator.setup();

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Clears the LCD screen
  resetLcd(lcd);
  lcd.setCursor(0, 0);
  lcd.print("Waiting for");
  lcd.setCursor(0, 1);
  lcd.print("input...");
}


unsigned int endSwitchValueReadCount = 0;
bool endSwitchPressed = false;

void loop() {
  int endSwitchValue = analogRead(__END_SWITCH_PIN__);
  bool buttonPressed = digitalRead(__CONTROL_MOTOR_PIN__);
  endSwitchPressed = endSwitchValue > __LED_THRESHOLD__; 

  if (buttonPressed) {
    currentStepper = &startMotorTimer;
    currentStepper->reset();
  } else if (endSwitchPressed) {
    currentStepper = &stopMotorTimer;
    currentStepper->reset();
  }

  digitalWrite(__LED_PIN__, endSwitchPressed);
  currentStepper->loop();

  if (endSwitchPressed) {

  }
  
  delay(__LOOP_DELAY_MS__);
}


void deccelerate() {
  unsigned short newSpeed = max(motorSpeed - __ACCELERATION__, __MIN_MOTOR_SPEED__);
  motorSpeed = newSpeed;
}

void resetLcd(LiquidCrystal& lcd) {
  lcd.clear();
  lcd.setCursor(0, 0);
}

mz::MotorControlState parseMotorControlState(bool moveMotor, bool motorIsMoving) {
    if (moveMotor && motorIsMoving)   return mz::MotorControlState::MOTOR_CONTROL_MOVING;
    if (!moveMotor && motorIsMoving)  return mz::MotorControlState::MOTOR_CONTROL_HALTING;
    if (moveMotor && !motorIsMoving)  return mz::MotorControlState::MOTOR_CONTROL_STARTING;
    /* else */                        return mz::MotorControlState::MOTOR_CONTROL_IDLE;
}

void haltMotor() {
  motor.setSpeed(0);
  motor.stop();
  motorSpeed = 0;
}

void startMotor() {
  // reset motor speed
  motorSpeed = endSwitchPressed ? __MAX_MOTOR_SPEED_SWITCH_ON__ : __MAX_MOTOR_SPEED_SWITCH_OFF__; 
  accelerator.setup();
  motor.backward();
}


#elif __PROGRAM_TYPE__ == __TIMER_W_MOTOR_N_LCD_W_STATE_MACHINE_POC__

/* On button press start a countdown of 5 seconds */
/* Display a message on the LCD screen with the countdown */
/* After 5 seconds, move the motor */
/* When the end-switch signal triggers, stop the motor */

#include <L298N.h>
#include "Stepper.h"
#include <limits.h>
#include <LiquidCrystal.h>
#include "MotorControlState.h"
#include "DispenserState.h"
#include "DispenserInput.h"
#include "MotorButtonValue.h"

#define __LOOP_DELAY_MS__ 5


// Motor Pin definition
// Create one motor instance
#define __MOTOR_ENABLE_PIN__ 3
#define __MOTOR_INI1_PIN__ 4
#define __MOTOR_INI2_PIN__ 5

#define __MAX_MOTOR_SPEED_SWITCH_OFF__ 150 // initial motor speed when end switch is NOT pressed
#define __MAX_MOTOR_SPEED_SWITCH_ON__ 220 // initial motor speed when end switch IS pressed
#define __MIN_MOTOR_SPEED__ 80
#define __ACCELERATION__ 5
void deccelerate();
mz::Stepper accelerator(&millis, &deccelerate, 25L, UINT_MAX);


/* Definition of LCD pins */
/* Problem with pin_11 and the motor module */
/* Avoid using pin_11 */
// Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
#define __RS_PIN__ 6
#define __EN_PIN__ 7
#define __D4_PIN__ 8
#define __D5_PIN__ 9
#define __D6_PIN__ 12
#define __D7_PIN__ 13


/* Control pins */
#define __END_SWITCH_PIN__ A5
#define __LED_PIN__ A3
#define __CONTROL_MOTOR_PIN__ A4
#define __LED_THRESHOLD__ 50
#define __MOTOR_BUTTON_LONG_PRESS_THRESHOLD_MS__ 1000L


/* <GLOBAL_STATE> ================================================================================ */

L298N motor(__MOTOR_ENABLE_PIN__, __MOTOR_INI1_PIN__, __MOTOR_INI2_PIN__);
unsigned short motorSpeed = __MAX_MOTOR_SPEED_SWITCH_ON__;
bool motorButtonPressedPrevious = false;
unsigned long motorButtonPressStartTime = 0L;

LiquidCrystal lcd(__RS_PIN__, __EN_PIN__, __D4_PIN__, __D5_PIN__, __D6_PIN__, __D7_PIN__);

mz::DispenserState dispenserState = mz::DispenserState::DISPENSER_IDLE;
mz::DispenserInput dispenserInput(
  false, // endSwitchPressed
  false  // motorControlButtonPressed
);

/* </GLOBAL_STATE> ================================================================================ */

mz::DispenserInput pollSensors();

void motorHalt();
void motorStart();

void lcdReset();
void lcd2Lines(const char* line0, const char* line1);

void dispenserStateIdleLoop();
void dispenserStateForceMoveMotorLoop();
void dispenserStateCountdown();


void setup() {
  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}
  Serial.println("__TIMER_W_MOTOR_N_LCD_W_STATE_MACHINE_POC__ ready to go");

  /* Shortcut of pinMode(X,INPUT);digitalWrite(X,HIGH); */
  pinMode(__CONTROL_MOTOR_PIN__, INPUT);
  pinMode(__END_SWITCH_PIN__, INPUT);
  pinMode(__LED_PIN__, OUTPUT);

  accelerator.setup();

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Clears the LCD screen
  lcdReset();
  lcd2Lines("Waiting for", "input...");
}

void loop() {
  dispenserInput = pollSensors();

  switch (dispenserState) {
    case mz::DispenserState::DISPENSER_IDLE:
      dispenserStateIdleLoop();
      break;
    case mz::DispenserState::DISPENSER_FORCE_MOVE_MOTOR:
      dispenserStateForceMoveMotorLoop();
      break;
    case mz::DispenserState::DISPENSER_COUNTDOWN:
      dispenserStateCountdown();
      break;
  };

  digitalWrite(__LED_PIN__, dispenserInput.isEndSwitchPressed());

  delay(__LOOP_DELAY_MS__);
}


void dispenserStateIdleLoop() {
  /* On IDLE state, the program is waiting for input */

  bool buttonPressedCurrent = dispenserInput.isMotorButtonPressed();
  unsigned long motorButtonPressTotalTime = millis() - motorButtonPressStartTime;
  bool longPress = motorButtonPressTotalTime > __MOTOR_BUTTON_LONG_PRESS_THRESHOLD_MS__;

  mz::MotorButtonValue motorButtonValue = mz::parseMotorButtonValue(
    buttonPressedCurrent, 
    motorButtonPressedPrevious, 
    longPress
  );

  switch(motorButtonValue) {
    case mz::MotorButtonValue::MOTOR_BUTTON_TOGGLE_ON:
    case mz::MotorButtonValue::MOTOR_BUTTON_TOGGLE_ON_:
      lcdReset(); lcd2Lines("Button", "pressed!");
      /* Start tracking button press start time */
      motorButtonPressStartTime = millis();
      break;

    case mz::MotorButtonValue::MOTOR_BUTTON_CONTINOUS_PRESS_LONG:
      lcd2Lines("Long    ", "press!    ");
      // trigger state transition
      dispenserState = mz::DispenserState::DISPENSER_FORCE_MOVE_MOTOR;
      break;

    case mz::MotorButtonValue::MOTOR_BUTTON_TOGGLE_OFF_SHORT_PRESS:
      // TODO :split actions here...
      lcdReset(); 
      dispenserState = mz::DispenserState::DISPENSER_COUNTDOWN;
      break;

    case mz::MotorButtonValue::MOTOR_BUTTON_TOGGLE_OFF_LONG_PRESS:
      // do nothing for now...
      lcdReset(); 
      break;

    case mz::MotorButtonValue::MOTOR_BUTTON_NO_PRESS:
    case mz::MotorButtonValue::MOTOR_BUTTON_NO_PRESS_:
      lcd2Lines("Waiting for", "input...");
      break;

    default:
      // Do nothing (for now...)
      break;
  }
  
  motorButtonPressedPrevious = buttonPressedCurrent;  
}

void dispenserStateForceMoveMotorLoop() {
  /* On DISPENSER_FORCE_MOVE_MOTOR state, the motor is forced to move until button is released */

  auto motorControlState = mz::parseMotorControlState(
    dispenserInput.isMotorButtonPressed(), 
    motor.isMoving()
  );

  switch (motorControlState) {
    case mz::MotorControlState::MOTOR_CONTROL_MOVING:
      // keep on moving
      accelerator.loop();
      motor.setSpeed(motorSpeed);
      motor.backward();
      break;
    
    case mz::MotorControlState::MOTOR_CONTROL_HALTING:
      // halt
      motor.setSpeed(0);
      motor.stop();
      motorSpeed = 0;
      // Back to being idle
      dispenserState = mz::DispenserState::DISPENSER_IDLE;
      break;

    case mz::MotorControlState::MOTOR_CONTROL_STARTING:
      // start moving

      // reset motor speed
      motorSpeed = __MAX_MOTOR_SPEED_SWITCH_ON__;
      motor.setSpeed(motorSpeed);
      accelerator.setup();
      motor.backward();
      break;

    default:
      __PNTLN("Motor state is IDLE... something is fishy");
      break;
  }  
}

#define __COUNTDOWN_ITERATIONS__ 5
#define __COUNTDOWN_PERIOD_MS__ 1000L
void dispenserStateCountdown() {
  lcdReset();

  for (int i = __COUNTDOWN_ITERATIONS__; i > 0; --i) {
    char buffer[16]; sprintf(buffer, "%d seconds", i);
    lcd2Lines(buffer, "remaining...");
    delay(__COUNTDOWN_PERIOD_MS__);
  }

  dispenserState = mz::DispenserState::DISPENSER_IDLE;
  lcdReset();
}

mz::DispenserInput pollSensors() {
  int endSwitchValue = analogRead(__END_SWITCH_PIN__);
  bool endSwitchPressed = endSwitchValue > __LED_THRESHOLD__; 
  bool motorButtonPressed = digitalRead(__CONTROL_MOTOR_PIN__);

  mz::DispenserInput input(endSwitchPressed, motorButtonPressed);
  return input;
}

void deccelerate() {
  unsigned short newSpeed = max(motorSpeed - __ACCELERATION__, __MIN_MOTOR_SPEED__);
  motorSpeed = newSpeed;
}

void lcdReset() {
  lcd.clear();
  lcd.setCursor(0, 0);
}

void lcd2Lines(const char* line0, const char* line1) {
  lcd.setCursor(0, 0); lcd.print(line0);
  lcd.setCursor(0, 1); lcd.print(line1);
}

void motorHalt() {
  motor.setSpeed(0);
  motor.stop();
  motorSpeed = 0;
}

void motorStart() {
  // reset motor speed
  motorSpeed = dispenserInput.isEndSwitchPressed() 
    ? __MAX_MOTOR_SPEED_SWITCH_ON__ 
    : __MAX_MOTOR_SPEED_SWITCH_OFF__; 
  accelerator.setup();
  motor.backward();
}



#endif
