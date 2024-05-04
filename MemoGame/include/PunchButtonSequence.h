#ifndef PUNCHBUTTONSEQUENCE_H
#define PUNCHBUTTONSEQUENCE_H

#include "MelodyBuzzer.h"

namespace mz {
    enum ButtonSequenceState {
        B_FRESH,          // Out of the oven
        B_INITIALIZED,    // Initialization done
        B_IDLE,           // Setup done & Waiting to begin
        B_IN_PROGRESS,
        B_WRONG_INPUT,
        B_COMPLETE,
        B_FAILED,
        B_DESTROYED,
        B_UNKNOWN
    };

    void initializeButtonSequence(
        mz::MelodyBuzzer* melodyBuzzer, // main buzzer
        uint8_t* buttonPins, // button pins array
        uint8_t buttonPinsSize, // size of button pins array
        int* notes, // notes for each button ; size must match buttonPinsSize
        uint8_t* buttonSequence, // array of game buttons to play
        uint8_t buttonSequenceSize, // size of button sequence array
        uint16_t buttonSequenceNoteDuration // durations of each note in milliseconds
    );
    
    // Returns true if there are no more items left in the sequence
    bool buttonSequenceDone();

    void setupButtonSequence();

    void destroyButtonSequence();

    void updateButtonSequence();

    ButtonSequenceState parseButtonSequenceState();
} // namespace mz

#endif