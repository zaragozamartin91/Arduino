#include "GameLed.h"
#include "PlayLedSequence.h"
#include <Arduino.h>
#include "MyMacros.h"

uint8_t PlayLedSequence_ledSequenceIndex; // keeps track of current led sequence item to play
bool PlayLedSequence_setupDone; // indicates if setup has been executed
bool PlayLedSequence_ledSequenceDestroyed; // indicates if led sequence memory has been released

mz::MelodyBuzzer* PlayLedSequence_melodyBuzzer; // main buzzer
uint8_t* PlayLedSequence_ledPins; // led pins array
uint8_t PlayLedSequence_ledPinsSize; // size of led pins array
int* PlayLedSequence_ledNotes; // notes for each led ; size must match ledPinsSize
uint8_t* PlayLedSequence_ledSequence; // array of game leds to play
uint8_t PlayLedSequence_ledSequenceSize; // size of led sequence array
uint16_t* PlayLedSequence_ledSequenceNoteDurations; // durations of each note in milliseconds


mz::GameLed* PlayLedSequence_gameLeds; // game leds


void PlayLedSequence_pinModeOutput(uint8_t pin) {pinMode(pin, OUTPUT);}
void PlayLedSequence_digitalWriteHigh(uint8_t pin) {digitalWrite(pin, HIGH);}
void PlayLedSequence_digitalWriteLow(uint8_t pin) {digitalWrite(pin, LOW);}

/* Plays note and moves sequence pointer forward */
void PlayLedSequence_playNote(unsigned int frequency, unsigned long duration);

void mz::initializeLedSequence(
    mz::MelodyBuzzer* melodyBuzzer, // main buzzer
    uint8_t* ledPins, // led pins array
    uint8_t ledPinsSize, // size of led pins array
    int* notes, // notes for each led ; size must match ledPinsSize
    uint8_t* ledSequence, // array of game leds to play
    uint8_t ledSequenceSize, // size of led sequence array
    uint16_t* ledSequenceNoteDurations // durations of each note in milliseconds
) {
    PlayLedSequence_ledSequenceIndex = 0; // reset sequence index on setup
    PlayLedSequence_setupDone = false; // reset setup flag
    PlayLedSequence_ledSequenceDestroyed = false; // reset led sequence destroyed flag
    
    PlayLedSequence_melodyBuzzer = melodyBuzzer;
    PlayLedSequence_ledPins = ledPins;
    PlayLedSequence_ledPinsSize = ledPinsSize;
    PlayLedSequence_ledNotes = notes;
    PlayLedSequence_ledSequence = ledSequence;
    PlayLedSequence_ledSequenceSize = ledSequenceSize;
    PlayLedSequence_ledSequenceNoteDurations = ledSequenceNoteDurations;

    PlayLedSequence_gameLeds = new mz::GameLed[ledPinsSize]; // default empty constructor used to initialize array
    for (size_t i = 0; i < ledPinsSize; i++) {
        // initialize game leds
        uint8_t pin = ledPins[i];
        int note = notes[i];
        mz::GameLed gl(pin, &PlayLedSequence_pinModeOutput, &PlayLedSequence_digitalWriteHigh, &PlayLedSequence_digitalWriteLow, &PlayLedSequence_playNote, note);
        PlayLedSequence_gameLeds[i] = gl; // invokes copy constructor
    }
}

void mz::setupLedSequence() {
    if(PlayLedSequence_setupDone) { return; } // nothing to do here...

    for (size_t i = 0; i < PlayLedSequence_ledPinsSize; i++) PlayLedSequence_gameLeds[i].setup();
    PlayLedSequence_setupDone = true;
}


bool mz::ledSequenceDone() {
    return PlayLedSequence_ledSequenceIndex >= PlayLedSequence_ledSequenceSize;
}

/* Obtains led to be played */
uint8_t PlayLedSequence_led2play() {
    return PlayLedSequence_ledSequence[PlayLedSequence_ledSequenceIndex];    
}

uint16_t PlayLedSequence_currentSequenceDuration() {
    return PlayLedSequence_ledSequenceNoteDurations[PlayLedSequence_ledSequenceIndex];
}

mz::GameLed* PlayLedSequence_currentGameLed() {
    uint8_t led2Play = PlayLedSequence_led2play();
    return &PlayLedSequence_gameLeds[led2Play];
}

void mz::destroyLedSequence() {
    if(PlayLedSequence_ledSequenceDestroyed) { return; } // nothing to do here
    delete[] PlayLedSequence_gameLeds;
    PlayLedSequence_ledSequenceDestroyed = true;
}

void mz::updateLedSequence() {
    if(ledSequenceDone()) { return; } // nothing to do here

    /* Pass durations as parameters */
    uint16_t duration = PlayLedSequence_currentSequenceDuration();
    PlayLedSequence_currentGameLed()->turnOn(duration);
}

uint8_t mz::getLedSequenceIndex() {
    return PlayLedSequence_ledSequenceIndex;
}

void PlayLedSequence_playNoteCallback(unsigned int, unsigned long, unsigned long, unsigned long) {
    PlayLedSequence_currentGameLed()->turnOff();
    PlayLedSequence_ledSequenceIndex++;
}

void PlayLedSequence_playNote(unsigned int frequency, unsigned long duration) {
  PlayLedSequence_melodyBuzzer->playAsync(frequency, duration, &PlayLedSequence_playNoteCallback);
};

