#include "PunchButtonSequence.h"
#include "GameButton.h"
#include <Arduino.h>
#include "MyMacros.h"

static mz::MelodyBuzzer* _pbs_melodyBuzzer; // main buzzer
static uint8_t* _pbs_buttonPins; // button pins array
static uint8_t _pbs_buttonPinsSize; // size of button pins array
static int* _pbs_notes; // notes for each button ; size must match buttonPinsSize
static uint8_t* _pbs_buttonSequence; // array of game buttons to play
static uint8_t _pbs_buttonSequenceSize; // size of button sequence array
static uint16_t _pbs_buttonSequenceNoteDuration; // durations of each note in milliseconds


static mz::GameButton* _pbs_gameButtons;
static uint8_t _pbs_buttonSequenceIndex; // keeps track of position in button index

static bool _pbs_sequenceInitialized = false;
static bool _pbs_sequenceSetup;
static bool _pbs_sequenceInProgress;
static bool _pbs_sequenceDestroyed;
static bool _pbs_wrongInputDetected;


void _pbs_pinModeIn(uint8_t p) { pinMode(p, INPUT); };
int _pbs_digitalRead(int p) {return digitalRead(p);}

// Temp solution to tie buttons to PRECISE callbacks
void _pbs_buttonX_pressed(int buttonId, int pressCount);
void _pbs_button0_pressed(int pressCount) { _pbs_buttonX_pressed(0, pressCount); };
void _pbs_button1_pressed(int pressCount) { _pbs_buttonX_pressed(1, pressCount); };
void _pbs_button2_pressed(int pressCount) { _pbs_buttonX_pressed(2, pressCount); };

void(*_pbs_buttonPressCallbacks[])(int) = { &_pbs_button0_pressed, &_pbs_button1_pressed, &_pbs_button2_pressed };

void mz::initializeButtonSequence(
        mz::MelodyBuzzer* melodyBuzzer, // main buzzer
        uint8_t* buttonPins, // button pins array
        uint8_t buttonPinsSize, // size of button pins array
        int* notes, // notes for each button ; size must match buttonPinsSize
        uint8_t* buttonSequence, // array of game buttons to play
        uint8_t buttonSequenceSize, // size of button sequence array
        uint16_t buttonSequenceNoteDuration // durations of each note in milliseconds
) {
    if (buttonPinsSize > MAX_BUTTONS_AND_LEDS) {
        debugPrint("System does not support that many buttons currently", ".");
        debugPrint("MAX_BUTTONS_AND_LEDS", MAX_BUTTONS_AND_LEDS);
        return;
    };

    _pbs_melodyBuzzer = melodyBuzzer;
    _pbs_buttonPins = buttonPins;
    _pbs_buttonPinsSize = buttonPinsSize;
    _pbs_notes = notes;
    _pbs_buttonSequence = buttonSequence;
    _pbs_buttonSequenceSize = buttonSequenceSize;
    _pbs_buttonSequenceNoteDuration = buttonSequenceNoteDuration;


    _pbs_sequenceSetup = false;
    _pbs_sequenceInProgress = false;
    _pbs_sequenceDestroyed = false;
    _pbs_wrongInputDetected = false;
    _pbs_buttonSequenceIndex = 0;


    _pbs_gameButtons = new mz::GameButton[buttonPinsSize];
    for(int i = 0 ; i < buttonPinsSize ; i++) {
        mz::GameButton gb(buttonPins[i], &_pbs_pinModeIn, &_pbs_digitalRead, _pbs_buttonPressCallbacks[i]);
        _pbs_gameButtons[i] = gb; // copy constructor invoked
    }

    _pbs_sequenceInitialized = true;
}

mz::ButtonSequenceState mz::parseButtonSequenceState() {
    if(!_pbs_sequenceInitialized)                           return B_FRESH;
    if(_pbs_sequenceDestroyed)                              return B_DESTROYED;
    if(mz::buttonSequenceDone() && _pbs_wrongInputDetected) return B_FAILED;
    if(mz::buttonSequenceDone())                            return B_COMPLETE;
    if(_pbs_sequenceInProgress && _pbs_wrongInputDetected)  return B_WRONG_INPUT;
    if(_pbs_sequenceInProgress)                             return B_IN_PROGRESS;
    if(_pbs_sequenceSetup)                                  return B_IDLE;
    if(_pbs_sequenceInitialized)                            return B_INITIALIZED;
    /* Otherwise... */                                      return B_UNKNOWN;
}

void mz::updateButtonSequence() {
    if(mz::buttonSequenceDone()) return; // nothing to do here
    for (int i = 0; i < _pbs_buttonPinsSize; i++) { _pbs_gameButtons[i].update(); }
}

void mz::setupButtonSequence() {
    if (_pbs_sequenceSetup) return; // nothing to do here
    for (int i = 0; i < _pbs_buttonPinsSize; i++) { _pbs_gameButtons[i].setup(); }
    _pbs_sequenceSetup = true;
}

bool mz::buttonSequenceDone() { return _pbs_buttonSequenceIndex >= _pbs_buttonSequenceSize; }

uint8_t _pbs_expectedInput() { return _pbs_buttonSequence[_pbs_buttonSequenceIndex]; }

int _pbs_expectedNote(int buttonId) { 
    return _pbs_notes[buttonId]; 
}

void _pbs_stepSequence() { ++_pbs_buttonSequenceIndex; }

void _pbs_buttonX_pressed(int buttonId, int pressCount) {
    if (mz::buttonSequenceDone()) return; // nothing to do here...

    _pbs_sequenceInProgress = true;


    debugPrint("Current sequence index: ", _pbs_buttonSequenceIndex);
    debugPrint("Button pressed: ", buttonId);
    debugPrint("expectedInput: ", _pbs_expectedInput());
    auto expectedNote = _pbs_expectedNote(buttonId);
    debugPrint("expectedNote: ", expectedNote);
    
    _pbs_melodyBuzzer->playAsync(expectedNote, _pbs_buttonSequenceNoteDuration); // play note
    _pbs_wrongInputDetected = _pbs_wrongInputDetected || buttonId != _pbs_expectedInput(); // detect wrong input
    _pbs_stepSequence(); // step sequence
}

void mz::destroyButtonSequence() {
    if (_pbs_sequenceDestroyed) { return; } // nothing to do here
    delete[] _pbs_gameButtons;
    _pbs_sequenceDestroyed = true;
}
