#include <Arduino.h>

const int BUZZER_PIN = PIN6;
const long BAUD_RATE = 9600;
const int BUZZER_POWER = 64;
const int TONE_CYCLES = 500;
String validInputs[] = {"HIGH", "LOW", "OFF"};

void waitForInput();

bool inputIsValid(String& userInput);

void playTone(int delayTime, int cycles);

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(BAUD_RATE);
  while(!Serial) {}
}

void loop() {
  String userInput;
  for(boolean validInput = false;  !validInput ; validInput = inputIsValid(userInput)) {
    Serial.println("Pick a tone: HIGH, LOW, OFF");
    waitForInput();
    userInput = Serial.readString();
  }

  Serial.print("Oh... so you want a "); Serial.print(userInput); Serial.println(" tone eh...");

  userInput.toLowerCase();
  switch (userInput.charAt(0)) {
  case 'o':
    digitalWrite(BUZZER_PIN, LOW);
    break;

  case 'h':
    playTone(1, TONE_CYCLES);
    break;
  
  default:
    playTone(5, TONE_CYCLES);
    break;
  }
}

void waitForInput() {
  while(!Serial.available()){};
}

bool inputIsValid(String& userInput) {
  if(userInput.equalsIgnoreCase("")) { return false; }

  String ms = "";
  for(int i = 0; i < 3 ; i++) { ms.concat(validInputs[i]); }
  ms.toLowerCase();

  String uic = userInput.c_str();
  uic.toLowerCase();
  return ms.indexOf(uic) >= 0;
}

void playTone(int delayTime, int cycles) {
    int netCycles = cycles / delayTime;

    // the tone of the active buzzer can sort off be changed by adding delays
    // this means adjusting the frequency
    for(int i = 0 ; i < netCycles ; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(delayTime);
      digitalWrite(BUZZER_PIN, LOW);
      delay(delayTime);
    }
}