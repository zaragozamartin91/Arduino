#ifndef READBUTTONSEQUENCE_H
#define READBUTTONSEQUENCE_H

#include "MelodyBuzzer.h"

namespace mz {

    /* Initializes variables and memory for playing led sequence */
    void initializeButtonSequence(
        mz::MelodyBuzzer* melodyBuzzer, // main buzzer
        uint8_t* buttonPins, // button pins array
        uint8_t buttonPinsSize, // size of led pins array
        int* notes, // notes for each led ; size must match ledPinsSize
        uint8_t* ledSequence, // array of game leds to play
        uint8_t ledSequenceSize // size of led sequence array
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