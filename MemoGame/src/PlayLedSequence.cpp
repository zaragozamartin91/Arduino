#include "GameLed.h"
#include "PlayLedSequence.h"
#include <Arduino.h>
#include "MyMacros.h"

/* Led sequence global state */

uint8_t _pls_ledSequenceIndex; // keeps track of current led sequence item to play
bool _pls_ledSequenceInitialized = false; // indicates if led sequence has been initialized
bool _pls_ledSequenceRunning; // indicates if led sequence is running
bool _pls_ledSequenceSetup; // indicates if setup has been executed
bool _pls_ledSequenceDestroyed; // indicates if led sequence memory has been released

mz::MelodyBuzzer* _pls_melodyBuzzer; // main buzzer
uint8_t* _pls_ledPins; // led pins array
uint8_t _pls_ledPinsSize; // size of led pins array
int* _pls_ledNotes; // notes for each led ; size must match ledPinsSize
uint8_t* _pls_ledSequence; // array of game leds to play
uint8_t _pls_ledSequenceSize; // size of led sequence array
uint16_t* _pls_ledSequenceNoteDurations; // durations of each note in milliseconds


mz::GameLed* _pls_gameLeds; // game leds


void _pls_pinModeOutput(uint8_t pin) {pinMode(pin, OUTPUT);}
void _pls_digitalWriteHigh(uint8_t pin) {digitalWrite(pin, HIGH);}
void _pls_digitalWriteLow(uint8_t pin) {digitalWrite(pin, LOW);}

/* Plays note and moves sequence pointer forward */
void _pls_playNote(unsigned int frequency, unsigned long duration);

void mz::initializeLedSequence(
    mz::MelodyBuzzer* melodyBuzzer, // main buzzer
    uint8_t* ledPins, // led pins array
    uint8_t ledPinsSize, // size of led pins array
    int* notes, // notes for each led ; size must match ledPinsSize
    uint8_t* ledSequence, // array of game leds to play
    uint8_t ledSequenceSize, // size of led sequence array
    uint16_t* ledSequenceNoteDurations // durations of each note in milliseconds
) {
    _pls_ledSequenceIndex = 0; // reset sequence index on setup
    _pls_ledSequenceRunning = false; // reset running flag
    _pls_ledSequenceSetup = false; // reset setup flag
    _pls_ledSequenceDestroyed = false; // reset led sequence destroyed flag
    
    _pls_melodyBuzzer = melodyBuzzer;
    _pls_ledPins = ledPins;
    _pls_ledPinsSize = ledPinsSize;
    _pls_ledNotes = notes;
    _pls_ledSequence = ledSequence;
    _pls_ledSequenceSize = ledSequenceSize;
    _pls_ledSequenceNoteDurations = ledSequenceNoteDurations;

    _pls_gameLeds = new mz::GameLed[ledPinsSize]; // default empty constructor used to initialize array
    for (size_t i = 0; i < ledPinsSize; i++) {
        // initialize game leds
        uint8_t pin = ledPins[i];
        int note = notes[i];
        mz::GameLed gl(pin, &_pls_pinModeOutput, &_pls_digitalWriteHigh, &_pls_digitalWriteLow, &_pls_playNote, note);
        _pls_gameLeds[i] = gl; // invokes copy constructor
    }

    _pls_ledSequenceInitialized = true;
}

void mz::setupLedSequence() {
    if(_pls_ledSequenceSetup) { return; } // nothing to do here...

    for (size_t i = 0; i < _pls_ledPinsSize; i++) _pls_gameLeds[i].setup();
    _pls_ledSequenceSetup = true;
}

bool mz::ledSequenceDone() {
    return _pls_ledSequenceIndex >= _pls_ledSequenceSize;
}

/* Obtains led to be played */
uint8_t _pls_led2play() {
    return _pls_ledSequence[_pls_ledSequenceIndex];    
}

uint16_t _pls_currentSequenceDuration() {
    return _pls_ledSequenceNoteDurations[_pls_ledSequenceIndex];
}

mz::GameLed* _pls_currentGameLed() {
    uint8_t led2Play = _pls_led2play();
    return &_pls_gameLeds[led2Play];
}

void mz::destroyLedSequence() {
    if(_pls_ledSequenceDestroyed) { return; } // nothing to do here
    delete[] _pls_gameLeds;
    _pls_ledSequenceDestroyed = true;
}

void mz::updateLedSequence() {
    if(ledSequenceDone()) { return; } // nothing to do here

    /* Pass durations as parameters */
    uint16_t duration = _pls_currentSequenceDuration();
    _pls_currentGameLed()->turnOn(duration);
    _pls_ledSequenceRunning = true; // indicate that led sequence is running
}

uint8_t mz::getLedSequenceIndex() {
    return _pls_ledSequenceIndex;
}

mz::LedSequenceState mz::parseLedSequenceState() {
    if(_pls_ledSequenceDestroyed)   /* ===> */ return DESTROYED;
    if(mz::ledSequenceDone())       /* ===> */ return FINISHED;
    if(_pls_ledSequenceRunning)     /* ===> */ return RUNNING;
    if(_pls_ledSequenceSetup)       /* ===> */ return READY;
    if(_pls_ledSequenceInitialized) /* ===> */ return INITIALIZED;
    /* OTHERWISE                       ===> */ return FRESH;
}

void _pls_playNoteCallback(unsigned int, unsigned long, unsigned long, unsigned long) {
    _pls_currentGameLed()->turnOff();
    _pls_ledSequenceIndex++;
}

void _pls_playNote(unsigned int frequency, unsigned long duration) {
    _pls_melodyBuzzer->playAsync(frequency, duration, &_pls_playNoteCallback);
};

