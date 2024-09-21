#include <Arduino.h>

#define _CALIBRATE_REMOTE_CONTROL 1
#define _CONTROL_MOTORS_WITH_REMOTE 2
#define _ANALYSE_PINS 3

#define _PROGRAM_TYPE _CONTROL_MOTORS_WITH_REMOTE


// ========================================================================

#if _PROGRAM_TYPE == _CALIBRATE_REMOTE_CONTROL

#include <IRremote.h>

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

#define _LOOP_DELAY 500
unsigned long loopDelayTime;

void loop() {
  int signalDecoded = irrecv.decode(&results);

  // have we received an IR signal?
  if (signalDecoded) {
    translateIR(); 
    delay(_LOOP_DELAY); // Do not get immediate repeat
    irrecv.resume(); // receive the next value
  }

  loopDelayTime = signalDecoded ? 0 : _LOOP_DELAY;
  delay(loopDelayTime);
} /* --(end main loop )-- */

#elif _PROGRAM_TYPE == _CONTROL_MOTORS_WITH_REMOTE

#include <L298NX2.h>
#include <IRremote.h>

#define STDBY_PIN 13
#define _GREEN_PIN 2

// Pin definition
// WATCH OUT: THIS PIN DEFINITION IS VERY SPECIFIC
// NOTE: THERE IS A PROBLEM WITH PIN 11 
// THERE ARE CONFLICTS WITH THE TIMERS AND THE MOTOR vs IR libraries: https://electronics.stackexchange.com/questions/511633/unable-to-use-ir-receiver-and-motor-shield-together
const unsigned int PWMA = 6;
const unsigned int _A12 = 7;
const unsigned int _A11 = 8;
 
const unsigned int _B11 = 9;
const unsigned int _B12 = 12;
const unsigned int PWMB = 10; // these have to be "squiggly pins"

// Initialize both motors
L298NX2 motors(PWMA, _A11,_A12, PWMB, _B11, _B12);

#define _RECV_PIN 4 // Signal Pin of IR receiver to Arduino Digital Pin
IRrecv irrecv(_RECV_PIN);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

#define _MOTOR_STD_SPEED 200

void handleIrSignal();


void setup() {
  pinMode(_GREEN_PIN, OUTPUT);
  digitalWrite(_GREEN_PIN, LOW);

  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}

    // IR setup
  irrecv.blink13(false);
  irrecv.enableIRIn(); 
}

#define _LOOP_DELAY 500
unsigned long loopDelayTime;

#define _MOTOR_ACTION_DURATION_MS 3000
unsigned long motorActionStartTimeMs = 0;
unsigned long motorActionEndTimeMs = 0;
unsigned long currentTimeMs;
bool motorsMoving;

void loop() {
  currentTimeMs = millis();
  motorsMoving = motors.isMovingA() || motors.isMovingB();
  if (currentTimeMs > motorActionEndTimeMs && motorsMoving) {
    motors.stop();
  }

  int signalDecoded = irrecv.decode(&results);

  // have we received an IR signal?
  if (signalDecoded) {
    handleIrSignal(); 
    delay(_LOOP_DELAY); // Do not get immediate repeat
    irrecv.resume(); // receive the next value
  }

  loopDelayTime = signalDecoded ? 0 : _LOOP_DELAY;
  delay(loopDelayTime);
}


void handleIrSignal() {
  // takes action based on IR code received
  // describing Remote IR codes 
  bool signalOk = false;

  switch(results.value) {
    case 0xFFA25D: 
      Serial.println("POWER");
      signalOk = true;
      motors.stop();
      break;

    case 0xFF02FD: // kit control 
    case 0x20DF22DD: // tv remote
      Serial.println("PAUSE | OK");
      signalOk = true;
      motors.stop();
      break;

    case 0xFFE01F:    // kit control 
    case 0x20DF827D:  // tv remote
      Serial.println("DOWN");
      signalOk = true;
      motors.setSpeed(_MOTOR_STD_SPEED);     
      motors.backward();
      motorActionStartTimeMs = millis();
      motorActionEndTimeMs = motorActionStartTimeMs + _MOTOR_ACTION_DURATION_MS;
      break;
    
    case 0xFF906F:    // kit control
    case 0x20DF02FD:  // tv remote
      Serial.println("UP");
      signalOk = true;
      motors.setSpeed(_MOTOR_STD_SPEED);    
      motors.forward();
      motorActionStartTimeMs = millis();
      motorActionEndTimeMs = motorActionStartTimeMs + _MOTOR_ACTION_DURATION_MS;
      break;

    case 0xFF22DD: // kit control 
    case 0x20DFE01F: // tv remote
      Serial.println("FAST_BACK | LEFT");
      signalOk = true;
      motors.setSpeedA(_MOTOR_STD_SPEED);    
      motors.forwardA();     
      motorActionStartTimeMs = millis();
      motorActionEndTimeMs = motorActionStartTimeMs + _MOTOR_ACTION_DURATION_MS;
      break;

    case 0xFFC23D: // kit control 
    case 0x20DF609F: // tv remote
      Serial.println("FAST_FORWARD | RIGHT");
      signalOk = true;  
      motors.setSpeedB(_MOTOR_STD_SPEED);    
      motors.forwardB();    
      motorActionStartTimeMs = millis();
      motorActionEndTimeMs = motorActionStartTimeMs + _MOTOR_ACTION_DURATION_MS; 
      break;

    case 0xFFFFFFFF: 
      Serial.println("REPEAT");     
      break;  

    default:
      Serial.print("other button: ");Serial.print(results.value, HEX);Serial.println();
      signalOk = false;
      break;
  }// End Case

  digitalWrite(_GREEN_PIN, signalOk);
}


#elif _PROGRAM_TYPE == _ANALYSE_PINS

#include <IRremote.h>

int receiver = 3; // Signal Pin of IR receiver to Arduino Digital Pin 2

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'


void handleIrSignal() {
  // takes action based on IR code received
  // describing Remote IR codes 

  switch(results.value) {
    case 0xFFA25D: 
    case 0xE318261B:
      Serial.println("POWER");
      break;

    case 0xFFE01F: 
      Serial.println("DOWN");          
      break;
    
    case 0xFF906F: 
      Serial.println("UP");
      break;

    case 0xFFFFFFFF: 
      Serial.println("REPEAT");     
      break;  

    default:
      Serial.print("other button: ");Serial.print(results.value, HEX);Serial.println();
  }// End Case
}


void setup() {
  // pinMode(STDBY_PIN, OUTPUT);
  // digitalWrite(STDBY_PIN, HIGH);

  // Used to display information
  Serial.begin(9600);

  // Wait for Serial Monitor to be opened
  while (!Serial) {}

    // IR setup
  irrecv.blink13(false);
  irrecv.enableIRIn(); 
}

void loop() {
  for (int pin = 2 ; pin <= 13 ; pin++) {
    auto pinValue = digitalRead(pin);

    Serial.print("Pin ");
    Serial.print(pin);
    Serial.print(" value = ");
    Serial.println(pinValue);
  }

  Serial.println();
  delay(5000);
}


#endif