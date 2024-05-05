// Connect led sequence and button sequence


#include <Arduino.h>
#include "music_notes.h" // including the library with the frequencies of the note 
#include "MelodyBuzzer.h"
#include "PlayLedSequence.h"
#include "MyMacros.h"
#include "GameButton.h"
#include "PunchButtonSequence.h"
#include "PlayWrongInputMelody.h"

uint8_t PHOTO_SEED_PIN=A1; // PIN OF THE PHOTO RESISTOR FEEDER
uint8_t BUZZER_PIN=PIN2;

uint8_t GREEN_BUTTON_PIN=PIN4;
uint8_t RED_BUTTON_PIN=PIN5;
uint8_t BLUE_BUTTON_PIN=PIN6;

uint8_t GREEN_LED_PIN = GREEN_BUTTON_PIN;
uint8_t RED_LED_PIN = RED_BUTTON_PIN;
uint8_t BLUE_LED_PIN = BLUE_BUTTON_PIN;


int LED_NOTES[] = { NOTE_C4, NOTE_AS4, NOTE_GS4 }; // 3 notes
uint8_t LED_PINS[] = { GREEN_LED_PIN, RED_LED_PIN, BLUE_LED_PIN}; // 3 "pins"
uint8_t LED_PINS_SIZE = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

mz::MelodyBuzzer melodyBuzzer(BUZZER_PIN, &tone, &noTone, &millis);

#define _FULL_LED_SEQUENCE_SIZE 100
uint8_t FULL_LED_SEQUENCE[_FULL_LED_SEQUENCE_SIZE];
uint8_t* ledSequence = FULL_LED_SEQUENCE;

#define _INITIAL_LED_SEQUENCE_SIZE 4
#define _INITIAL_LED_SEQUENCE_DURATION 500
uint8_t ledSequenceSize = _INITIAL_LED_SEQUENCE_SIZE; // initial led sequence size is 4
uint16_t ledSequenceDurations[_FULL_LED_SEQUENCE_SIZE];


mz::PlayWrongInputMelody playWrongInputMelody(&melodyBuzzer, LED_PINS, LED_PINS_SIZE);


void _seedRandomizer() {
    float photoRead = analogRead(PHOTO_SEED_PIN);
    debugPrint("photoRead: ", photoRead);
    float photoRead100 = photoRead * 100.;
    debugPrint("photoRead100: ", photoRead100);

    unsigned int seed = static_cast<int>(photoRead100);
    debugPrint("Using seed: ", seed);
    srand(seed);
}

void _randomizeLedSequence() {
  for (int i = 0 , r = rand() ; i < _FULL_LED_SEQUENCE_SIZE ; i++ , r = rand()) {
      FULL_LED_SEQUENCE[i] = r % LED_PINS_SIZE;
      ledSequenceDurations[i] = _INITIAL_LED_SEQUENCE_DURATION;
  }
}

void setup () {
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(PHOTO_SEED_PIN, INPUT);

    Serial.begin(9600);
    while(!Serial){};

    _seedRandomizer();
    _randomizeLedSequence();

    debugPrint("ledSequenceSize: ", ledSequenceSize);
}



void _initializeGameLedSequence() {
  debugPrint("Initializing led sequence", ".");
  mz::initializeLedSequence(
    &melodyBuzzer, LED_PINS, LED_PINS_SIZE, LED_NOTES, ledSequence, ledSequenceSize, ledSequenceDurations
  );
}

#define _BTN_SEQUENCE_DURATIONS 250
void _initializeGameButtonSequence() {
  debugPrint("Initializing button sequence", ".");
  mz::initializeButtonSequence(&melodyBuzzer, LED_PINS, LED_PINS_SIZE, LED_NOTES, ledSequence, ledSequenceSize, _BTN_SEQUENCE_DURATIONS);
}


void _restartGameLedSequence() {
  _randomizeLedSequence(); // regenerate led sequence
  ledSequenceSize = _INITIAL_LED_SEQUENCE_SIZE; // reset game
  _initializeGameLedSequence(); // re-create led sequence
}


#define _COMPLETE_SEQUENCE_DELAY 1000
#define _FAILED_SEQUENCE_DELAY 500
#define _RESTART_GAME_DELAY 1000
void _loopGameButtonSequence() {
  auto buttonSequenceState = mz::parseButtonSequenceState();

  switch (buttonSequenceState) {
    case mz::ButtonSequenceState::B_FRESH:
      _initializeGameButtonSequence();
      break;

    case mz::ButtonSequenceState::B_INITIALIZED:
      debugPrint("Setting up button sequence", ".");
      mz::setupButtonSequence();
      break;

    case mz::ButtonSequenceState::B_IDLE:
    case mz::ButtonSequenceState::B_IN_PROGRESS:
    case mz::ButtonSequenceState::B_WRONG_INPUT:
      mz::updateButtonSequence();
      break;

    case mz::ButtonSequenceState::B_COMPLETE:
      debugPrint("Button sequence complete", "!");
      mz::destroyButtonSequence();
      debugPrint("Button sequence destroyed", ".");
      delay(_COMPLETE_SEQUENCE_DELAY); // artificial delay of 1s
      ledSequenceSize++; // incrementing led sequence size
      _initializeGameLedSequence(); // re-create led sequence
      break;

    case mz::ButtonSequenceState::B_FAILED:
      debugPrint("Button sequence FAILED", "!");
      mz::destroyButtonSequence();
      debugPrint("Button sequence destroyed", ".");
      delay(_FAILED_SEQUENCE_DELAY); 
      playWrongInputMelody.initialize();
      playWrongInputMelody.setup();
      playWrongInputMelody.play([](){
        // on end execute =>
        delay(_RESTART_GAME_DELAY);
        _restartGameLedSequence();
      });
      break;

    case mz::ButtonSequenceState::B_DESTROYED:
      // 2nd round of button sequence input
      _initializeGameButtonSequence();
      break;

    default:
      // do nothing...
      break;
  }
}


void _loopGameLedSequence() {
  auto ledSequenceState = mz::parseLedSequenceState();

  switch (ledSequenceState) {
    case mz::LedSequenceState::FRESH:
      _initializeGameLedSequence();
      break;

    case mz::LedSequenceState::INITIALIZED:
      debugPrint("Setting up led sequence", ".");
      mz::setupLedSequence();
      break;

    case mz::LedSequenceState::READY:
    case mz::LedSequenceState::RUNNING:
      // debugPrint("Led sequence index: ", mz::getLedSequenceIndex());
      mz::updateLedSequence();
      break;

    case mz::LedSequenceState::FINISHED:
      debugPrint("Destroying led sequence",".");
      mz::destroyLedSequence();
      debugPrint("Led sequence destroyed",".");
      break;

    case mz::LedSequenceState::DESTROYED:
      _loopGameButtonSequence();
      break;
    
    default:
      // do nothing...
      break;
  }
}

#define _LOOP_DELAY_TIME 50
void loop() {
  if (playWrongInputMelody.melodyPlaying()) {
    playWrongInputMelody.update();
  } else {
    _loopGameLedSequence();
  }

  melodyBuzzer.update();
  delay(_LOOP_DELAY_TIME);
}


// ****************************************************************************************************



// Play wrong input melody POC

// #include <Arduino.h>
// #include "music_notes.h" // including the library with the frequencies of the note 
// #include "MelodyBuzzer.h"
// #include "PlayLedSequence.h"
// #include "MyMacros.h"
// #include "GameButton.h"
// #include "PlayWrongInputMelody.h"

// /* EXAMPLE code based on https://www.hackster.io/brambi001/play-any-song-with-arduino-passive-buzzer-000d52 written by Ilaria Brambilla - 25/09/2022 */

// uint8_t BUZZER_PIN=PIN2;

// uint8_t GREEN_BUTTON_PIN=PIN4;
// uint8_t RED_BUTTON_PIN=PIN5;
// uint8_t BLUE_BUTTON_PIN=PIN6;

// uint8_t GREEN_LED_PIN = GREEN_BUTTON_PIN;
// uint8_t RED_LED_PIN = RED_BUTTON_PIN;
// uint8_t BLUE_LED_PIN = BLUE_BUTTON_PIN;


// mz::MelodyBuzzer melodyBuzzer(BUZZER_PIN, &tone, &noTone, &millis);


// uint8_t LED_PINS[] = {
//   GREEN_LED_PIN,
//   RED_LED_PIN,
//   BLUE_LED_PIN
// }; 

// mz::PlayWrongInputMelody playWrongInputMelody(
//   &melodyBuzzer, LED_PINS, 3, []() { debugPrint("Sequence has ended", "."); }
// );

// void setup () {
//     pinMode(BUZZER_PIN, OUTPUT);

//     Serial.begin(9600);
//     while(!Serial){};

//     debugPrint("Initializing led sequence", ".");
//     playWrongInputMelody.initialize();

//     debugPrint("Setting up led sequence", ".");
//     playWrongInputMelody.setup();
// }


// int LOOP_DELAY_TIME = 100;

// void loop() {
//   playWrongInputMelody.update();
//   melodyBuzzer.update();
//   delay(LOOP_DELAY_TIME);
// }

// ****************************************************************************************************

// Play wrong input melody MANUAL POC

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

// Game Button POC

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
// mz::GameButton greenGameButton(GREEN_BUTTON_PIN, &pinModeInput, &digitalReadPin, &playGreenNote);

// void playBlueNote(int) { 
//   melodyBuzzer.playAsync(BLUE_NOTE, 500);
//   debugPrint("Playing buzzer", ".");  
// }
// mz::GameButton blueGameButton(BLUE_BUTTON_PIN, &pinModeInput, &digitalReadPin, &playBlueNote);


// mz::ToggleButton tb(
//   RED_BUTTON_PIN, 
//   [](int p){return digitalRead(p);}, 
//   [](int p){melodyBuzzer.playAsync(RED_NOTE, 1000);}
// );

// void setup () {
//     pinMode(BUZZER_PIN, OUTPUT);
//     pinMode(RED_BUTTON_PIN, INPUT);
//     greenGameButton.setup();
//     blueGameButton.setup();

//     Serial.begin(9600);
//     while(!Serial){};

//     debugPrint("Serial ready", "!");
// }


// int LOOP_DELAY_TIME = 100;

// void loop() {
//   melodyBuzzer.update();
//   greenGameButton.update();
//   blueGameButton.update();
//   tb.update();


//   auto gdr = digitalRead(GREEN_BUTTON_PIN);
//   debugPrint("gdr: ", gdr);


//   delay(LOOP_DELAY_TIME);
// }
