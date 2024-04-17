// #include "ToggleButton.h"
// #include "ReadButtonSequence.h"
// #include <Arduino.h>
// #include "MyMacros.h"

// /* Led sequence global state */

// uint8_t _rbs_ledSequenceIndex; // keeps track of current led sequence item to play
// bool _rbs_setupDone; // indicates if setup has been executed
// bool _rbs_ledSequenceDestroyed; // indicates if led sequence memory has been released

// mz::MelodyBuzzer* _rbs_melodyBuzzer; // main buzzer
// uint8_t* _rbs_buttonPins; // led pins array
// uint8_t _rbs_buttonPinsSize; // size of led pins array
// int* _rbs_ledNotes; // notes for each led ; size must match ledPinsSize
// uint8_t* _rbs_ledSequence; // array of game leds to play
// uint8_t _rbs_ledSequenceSize; // size of led sequence array


// mz::ToggleButton* _rbs_toggleButtons; // toggle buttons


// void _rbs_pinModeInput(uint8_t pin) {pinMode(pin, INPUT);}

// /* Plays note and moves sequence pointer forward */
// void _rbs_playNote(unsigned int frequency, unsigned long duration);

// void mz::initializeButtonSequence(
//         mz::MelodyBuzzer* melodyBuzzer, // main buzzer
//         uint8_t* buttonPins, // button pins array
//         uint8_t buttonPinsSize, // size of led pins array
//         int* notes, // notes for each led ; size must match ledPinsSize
//         uint8_t* ledSequence, // array of game leds to play
//         uint8_t ledSequenceSize // size of led sequence array
// ) {
//     _rbs_ledSequenceIndex = 0; // reset sequence index on setup
//     _rbs_setupDone = false; // reset setup flag
//     _rbs_ledSequenceDestroyed = false; // reset led sequence destroyed flag
    
//     _rbs_melodyBuzzer = melodyBuzzer;
//     _rbs_buttonPins = buttonPins;
//     _rbs_buttonPinsSize = buttonPinsSize;
//     _rbs_ledNotes = notes;
//     _rbs_ledSequence = ledSequence;
//     _rbs_ledSequenceSize = ledSequenceSize;

//     _rbs_toggleButtons = new mz::ToggleButton[buttonPinsSize]; // default empty constructor used to initialize array
//     for (size_t i = 0; i < buttonPinsSize; i++) {
//         // initialize game leds

//         int note = notes[i];
//         mz::GameLed gl(pin, &_rbs_pinModeOutput, &_rbs_digitalWriteHigh, &_rbs_digitalWriteLow, &_rbs_playNote, note);
//         _rbs_toggleButtons[i] = gl; // invokes copy constructor
//     }
// }

// void mz::setupLedSequence() {
//     if(_rbs_setupDone) { return; } // nothing to do here...

//     for (size_t i = 0; i < _rbs_buttonPinsSize; i++) _rbs_toggleButtons[i].setup();
//     _rbs_setupDone = true;
// }


// bool mz::ledSequenceDone() {
//     return _rbs_ledSequenceIndex >= _rbs_ledSequenceSize;
// }

// /* Obtains led to be played */
// uint8_t _rbs_led2play() {
//     return _rbs_ledSequence[_rbs_ledSequenceIndex];    
// }

// uint16_t _rbs_currentSequenceDuration() {
//     return _rbs_ledSequenceNoteDurations[_rbs_ledSequenceIndex];
// }

// mz::GameLed* _rbs_currentGameLed() {
//     uint8_t led2Play = _rbs_led2play();
//     return &_rbs_toggleButtons[led2Play];
// }

// void mz::destroyLedSequence() {
//     if(_rbs_ledSequenceDestroyed) { return; } // nothing to do here
//     delete[] _rbs_toggleButtons;
//     _rbs_ledSequenceDestroyed = true;
// }

// void mz::updateLedSequence() {
//     if(ledSequenceDone()) { return; } // nothing to do here

//     /* Pass durations as parameters */
//     uint16_t duration = _rbs_currentSequenceDuration();
//     _rbs_currentGameLed()->turnOn(duration);
// }

// uint8_t mz::getLedSequenceIndex() {
//     return _rbs_ledSequenceIndex;
// }

// void _rbs_playNoteCallback(unsigned int, unsigned long, unsigned long, unsigned long) {
//     _rbs_currentGameLed()->turnOff();
//     _rbs_ledSequenceIndex++;
// }

// void _rbs_playNote(unsigned int frequency, unsigned long duration) {
//   _rbs_melodyBuzzer->playAsync(frequency, duration, &_rbs_playNoteCallback);
// };

