#include <Arduino.h>
#include "music_notes.h" // including the library with the frequencies of the note 
#include "MelodyBuzzer.h"
#include "PlayLedSequence.h"
#include "MyMacros.h"
#include "GameButton.h"
#include "PlayWrongInputMelody.h"

/* EXAMPLE code based on https://www.hackster.io/brambi001/play-any-song-with-arduino-passive-buzzer-000d52 written by Ilaria Brambilla - 25/09/2022 */

uint8_t BUZZER_PIN=PIN2;

uint8_t GREEN_BUTTON_PIN=PIN4;
uint8_t RED_BUTTON_PIN=PIN5;
uint8_t BLUE_BUTTON_PIN=PIN6;

uint8_t GREEN_LED_PIN = GREEN_BUTTON_PIN;
uint8_t RED_LED_PIN = RED_BUTTON_PIN;
uint8_t BLUE_LED_PIN = BLUE_BUTTON_PIN;


mz::MelodyBuzzer melodyBuzzer(BUZZER_PIN, &tone, &noTone, &millis);


uint8_t LED_PINS[] = {
  GREEN_LED_PIN,
  RED_LED_PIN,
  BLUE_LED_PIN
}; 

mz::PlayWrongInputMelody playWrongInputMelody(
  &melodyBuzzer, LED_PINS, 3, []() { debugPrint("Sequence has ended", "."); }
);

void setup () {
    pinMode(BUZZER_PIN, OUTPUT);

    Serial.begin(9600);
    while(!Serial){};

    debugPrint("Initializing led sequence", ".");
    playWrongInputMelody.initialize();

    debugPrint("Setting up led sequence", ".");
    playWrongInputMelody.setup();
}


int LOOP_DELAY_TIME = 100;

void loop() {
  playWrongInputMelody.update();
  melodyBuzzer.update();
  delay(LOOP_DELAY_TIME);
}

// ****************************************************************************************************

// #include <Arduino.h>
// #include "music_notes.h" // including the library with the frequencies of the note 
// #include "MelodyBuzzer.h"
// #include "PlayLedSequence.h"
// #include "MyMacros.h"
// #include "GameButton.h"

// /* EXAMPLE code based on https://www.hackster.io/brambi001/play-any-song-with-arduino-passive-buzzer-000d52 written by Ilaria Brambilla - 25/09/2022 */

// uint8_t BUZZER_PIN=PIN2;

// uint8_t GREEN_BUTTON_PIN=PIN4;
// uint8_t RED_BUTTON_PIN=PIN5;
// uint8_t BLUE_BUTTON_PIN=PIN6;

// uint8_t GREEN_LED_PIN = GREEN_BUTTON_PIN;
// uint8_t RED_LED_PIN = RED_BUTTON_PIN;
// uint8_t BLUE_LED_PIN = BLUE_BUTTON_PIN;


// int LED_NOTES[] = {
//   NOTE_C4,
//   NOTE_AS4,
//   NOTE_GS4,
//   NOTE_G4,
//   NOTE_GS4,
//   NOTE_F4,
//   NOTE_DS4,
//   NOTE_D4,
//   NOTE_C4,
//   NOTE_C4
// }; // 10 notes


// uint8_t LED_PINS[] = {
//   GREEN_LED_PIN,
//   RED_LED_PIN,
//   BLUE_LED_PIN,
//   GREEN_LED_PIN,
//   RED_LED_PIN,
//   BLUE_LED_PIN,
//   GREEN_LED_PIN,
//   RED_LED_PIN,
//   BLUE_LED_PIN,
//   GREEN_LED_PIN
// }; // 10 "pins"
// uint8_t LED_PINS_SIZE = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

// mz::MelodyBuzzer melodyBuzzer(BUZZER_PIN, &tone, &noTone, &millis);

// uint8_t ledSequence[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
// uint8_t ledSequenceSize = sizeof(ledSequence) / sizeof(ledSequence[0]);
// uint16_t ledSequenceDurations[] = {
//   100,
//   100,
//   100,
//   100,
//   100,
//   100,
//   100,
//   100,
//   100,
//   100
// }; // 10 durations


// void setup () {
//     pinMode(BUZZER_PIN, OUTPUT);

//     Serial.begin(9600);
//     while(!Serial){};

//     debugPrint("ledSequenceSize: ", ledSequenceSize);

//     debugPrint("Initializing led sequence", ".");
//     mz::initializeLedSequence(
//       &melodyBuzzer, LED_PINS, LED_PINS_SIZE, LED_NOTES, ledSequence, ledSequenceSize, ledSequenceDurations
//     );

//     debugPrint("Setting up led sequence", ".");
//     mz::setupLedSequence();
// }


// int LOOP_DELAY_TIME = 100;

// void loop() {
//   auto ledSequenceState = mz::parseLedSequenceState();

//   switch (ledSequenceState) {
//     case mz::LedSequenceState::RUNNING:
//     case mz::LedSequenceState::READY:
//       debugPrint("Led sequence index: ", mz::getLedSequenceIndex());
//       mz::updateLedSequence();
//       break;

//     case mz::LedSequenceState::FINISHED:
//       debugPrint("Destroying led sequence",".");
//       mz::destroyLedSequence();
//       debugPrint("Led sequence destroyed",".");
//       break;      
    
//     default:
//       // do nothing...
//       break;
//   }

//   melodyBuzzer.update();
//   delay(LOOP_DELAY_TIME);
// }


// ****************************************************************************************************


// #include <Arduino.h>
// #include "music_notes.h" // including the library with the frequencies of the note 
// #include "MelodyBuzzer.h"
// #include "GameButton.h"
// #include "ToggleButton.h"
// #include "MyMacros.h"


// uint8_t BUZZER_PIN=PIN2;

// uint8_t GREEN_BUTTON_PIN=PIN4;
// uint8_t RED_BUTTON_PIN=PIN5;
// uint8_t BLUE_BUTTON_PIN=PIN6;

// uint8_t GREEN_LED_PIN = GREEN_BUTTON_PIN;
// uint8_t RED_LED_PIN = RED_BUTTON_PIN;
// uint8_t BLUE_LED_PIN = BLUE_BUTTON_PIN;

// uint8_t LED_PINS[] = {GREEN_LED_PIN, RED_LED_PIN, BLUE_LED_PIN};
// uint8_t LED_PINS_SIZE = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

// int GREEN_NOTE = NOTE_G4;
// int RED_NOTE = NOTE_E4;
// int BLUE_NOTE = NOTE_C4;


// mz::MelodyBuzzer melodyBuzzer(BUZZER_PIN, &tone, &noTone, &millis);

// void pinModeInput(uint8_t p) { pinMode(p, INPUT); }
// int digitalReadPin(int p) { return digitalRead(p); }
// void playGreenNote(int) { 
//   melodyBuzzer.playAsync(GREEN_NOTE, 500);
//   debugPrint("Playing buzzer", ".");  
// }
// mz::GameButton gameButton(GREEN_BUTTON_PIN, &pinModeInput, &digitalReadPin, &playGreenNote);


// mz::ToggleButton tb(
//   RED_BUTTON_PIN, 
//   [](int p){return digitalRead(p);}, 
//   [](int p){melodyBuzzer.playAsync(RED_NOTE, 1000);}
// );

// void setup () {
//     pinMode(BUZZER_PIN, OUTPUT);
//     pinMode(RED_BUTTON_PIN, INPUT);
//     gameButton.setup();

//     Serial.begin(9600);
//     while(!Serial){};

//     debugPrint("Serial ready", "!");
// }


// int LOOP_DELAY_TIME = 100;

// void loop() {
//   melodyBuzzer.update();
//   gameButton.update();
//   tb.update();


//   auto gdr = digitalRead(GREEN_BUTTON_PIN);
//   debugPrint("gdr: ", gdr);


//   delay(LOOP_DELAY_TIME);
// }
