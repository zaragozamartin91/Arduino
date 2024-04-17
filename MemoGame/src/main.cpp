#include <Arduino.h>
#include "music_notes.h" // including the library with the frequencies of the note 
#include "MelodyBuzzer.h"
#include "PlayLedSequence.h"
#include "MyMacros.h"
#include "StackArray.h"

/* EXAMPLE code based on https://www.hackster.io/brambi001/play-any-song-with-arduino-passive-buzzer-000d52 written by Ilaria Brambilla - 25/09/2022 */

uint16_t foo[] = {1,2,3};
mz::StackArray<3, uint16_t> sa(foo);

mz::StackArray<3, uint16_t> ba(static_cast<uint16_t>(1),static_cast<uint16_t>(2));


uint8_t BUZZER_PIN=PIN2;

uint8_t GREEN_BUTTON_PIN=PIN6;
uint8_t RED_BUTTON_PIN=9;
uint8_t BLUE_BUTTON_PIN=12;

uint8_t GREEN_LED_PIN = GREEN_BUTTON_PIN;
uint8_t RED_LED_PIN = RED_BUTTON_PIN;
uint8_t BLUE_LED_PIN = BLUE_BUTTON_PIN;

uint8_t LED_PINS[] = {GREEN_LED_PIN, RED_LED_PIN, BLUE_LED_PIN};
uint8_t LED_PINS_SIZE = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

int GREEN_NOTE = NOTE_G4;
int RED_NOTE = NOTE_E4;
int BLUE_NOTE = NOTE_C4;

int LED_NOTES[] = {GREEN_NOTE, RED_NOTE, BLUE_NOTE};

mz::MelodyBuzzer melodyBuzzer(BUZZER_PIN, &tone, &noTone, &millis);


uint8_t ledSequence[] = {0, 1, 2, 1, 2, 0, 2, 1, 0, 0, 0, 2};
uint8_t ledSequenceSize = sizeof(ledSequence) / sizeof(ledSequence[0]);
uint16_t ledSequenceDurations[] = {1000, 500, 1000, 250, 500, 1000, 300, 750, 250, 400, 700, 800};


void setup () {
    pinMode(BUZZER_PIN, OUTPUT);

    Serial.begin(9600);
    while(!Serial){};

    debugPrint("ledSequenceSize: ", ledSequenceSize);

    debugPrint("Initializing led sequence", ".");
    mz::initializeLedSequence(
      &melodyBuzzer, LED_PINS, LED_PINS_SIZE, LED_NOTES, ledSequence, ledSequenceSize, ledSequenceDurations
    );

    debugPrint("Setting up led sequence", ".");
    mz::setupLedSequence();

    debugPrint("ba[0] = ", ba[0]);
    debugPrint("sa[0] = ", sa[0]);
}


int LOOP_DELAY_TIME = 100;

void loop() {

  if (mz::ledSequenceDone()) {
    debugPrint("Destroying led sequence",".");
    mz::destroyLedSequence();
    debugPrint("Led sequence destroyed",".");
  } else {
    debugPrint("Led sequence index: ", mz::getLedSequenceIndex());
    mz::updateLedSequence();
  }

  melodyBuzzer.update();
  delay(LOOP_DELAY_TIME);
}



// #include <Arduino.h>
// #include "ToggleButton.h"

// int READ_PIN=PIN6;
// int LED_PIN=9;

// int LED_POWER_LEVELS[] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 255};
// int LED_POWER_LEVELS_L = sizeof(LED_POWER_LEVELS) / sizeof(LED_POWER_LEVELS[0]);

// int v_ledPowerLevel = 0;

// /* Declaring Toggle button */
// void dimLed(int pressCount) {
//   int powerLevelIndex = pressCount % LED_POWER_LEVELS_L;
//   v_ledPowerLevel = LED_POWER_LEVELS[powerLevelIndex];
// }
// int readButtonSignal(int pin) {return digitalRead(pin);}
// mz::ToggleButton v_toggle(READ_PIN, &readButtonSignal, &dimLed);

// int v_pressCount = 0;

// void setup() {
//   pinMode(READ_PIN, INPUT);
//   pinMode(LED_PIN, OUTPUT);

//   Serial.begin(9600);
//   while(!Serial){};
// }

// void loop() {
//   v_toggle.update();

//   v_pressCount = v_toggle.getPressCount();
//   Serial.print("v_pressCount: ");Serial.print(v_pressCount);Serial.print(" ; v_ledPowerLevel: ");Serial.println(v_ledPowerLevel);

//   analogWrite(LED_PIN, v_ledPowerLevel);

//   delay(100);
// }
