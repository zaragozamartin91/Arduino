#ifndef PLAYLEDSEQUENCE_H
#define PLAYLEDSEQUENCE_H

#include "MelodyBuzzer.h"

namespace mz {
    enum LedSequenceState {
        
    };

    /* Initializes variables and memory for playing led sequence */
    void initializeLedSequence(
        mz::MelodyBuzzer* melodyBuzzer, // main buzzer
        uint8_t* ledPins, // led pins array
        uint8_t ledPinsSize, // size of led pins array
        int* notes, // notes for each led ; size must match ledPinsSize
        uint8_t* ledSequence, // array of game leds to play
        uint8_t ledSequenceSize, // size of led sequence array
        uint16_t* ledSequenceNoteDurations // durations of each note in milliseconds
    );

    /* Runs setup on led sequence items */
    void setupLedSequence();

    /* Indicates whether led sequence is done playing */
    bool ledSequenceDone();

    /* Updates led sequence */
    void updateLedSequence();

    /* Obtains current index within sequence */
    uint8_t getLedSequenceIndex();

    /* Tears down led sequence */
    void destroyLedSequence();
}

#endif