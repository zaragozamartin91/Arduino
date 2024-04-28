#ifndef PUNCHBUTTONSEQUENCE_H
#define PUNCHBUTTONSEQUENCE_H

#include "MelodyBuzzer.h"

namespace mz {
    void initializeButtonSequence(
        mz::MelodyBuzzer* melodyBuzzer, // main buzzer
        uint8_t* buttonPins, // button pins array
        uint8_t buttonPinsSize, // size of button pins array
        int* notes, // notes for each button ; size must match buttonPinsSize
        uint8_t* buttonSequence, // array of game buttons to play
        uint8_t buttonSequenceSize, // size of button sequence array
        uint16_t* buttonSequenceNoteDurations // durations of each note in milliseconds
    );
    
    void destroyButtonSequence();
} // namespace mz

#endif