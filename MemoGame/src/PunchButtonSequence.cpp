#include "PunchButtonSequence.h"
#include "GameButton.h"

bool _pbs_sequenceDestroyed;
mz::GameButton* _pbs_buttons;

void mz::initializeButtonSequence(
        mz::MelodyBuzzer* melodyBuzzer, // main buzzer
        uint8_t* buttonPins, // button pins array
        uint8_t buttonPinsSize, // size of button pins array
        int* notes, // notes for each button ; size must match buttonPinsSize
        uint8_t* buttonSequence, // array of game buttons to play
        uint8_t buttonSequenceSize, // size of button sequence array
        uint16_t* buttonSequenceNoteDurations // durations of each note in milliseconds
) {

    _pbs_sequenceDestroyed = false;

    _pbs_buttons = new mz::GameButton[buttonPinsSize];
    
    
}


void mz::destroyButtonSequence() {
    if (_pbs_sequenceDestroyed) { return; } // nothing to do here
    delete[] _pbs_buttons;
    _pbs_sequenceDestroyed = true;
}