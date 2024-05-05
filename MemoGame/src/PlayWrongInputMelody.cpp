#include "PlayWrongInputMelody.h"
#include "PlayLedSequence.h"
#include "music_notes.h"


static int _PWIM_LED_NOTES[] = {
  NOTE_C4,
  NOTE_AS4,
  NOTE_GS4,
  NOTE_G4,
  NOTE_GS4,
  NOTE_F4,
  NOTE_DS4,
  NOTE_D4,
  NOTE_C4,
  NOTE_C4
}; // 10 notes 

static const int _PWIM_LED_NOTES_SIZE = sizeof(_PWIM_LED_NOTES) / sizeof(_PWIM_LED_NOTES[0]);

#define _PWIM_DUR 100
static uint16_t _PWIM_DURATIONS[] = { _PWIM_DUR, _PWIM_DUR, _PWIM_DUR, _PWIM_DUR, _PWIM_DUR, _PWIM_DUR, _PWIM_DUR, _PWIM_DUR, _PWIM_DUR, _PWIM_DUR }; 
// all durations are of 100ms 

mz::PlayWrongInputMelody::PlayWrongInputMelody(
    mz::MelodyBuzzer *melodyBuzzer,
    uint8_t *ledPinsParam,
    uint8_t ledPinsSize,
    void(*sequenceEndCallback)()
) :
melodyBuzzer{melodyBuzzer},
ledPinsSize{ledPinsSize},
sequenceEndCallback{sequenceEndCallback},
sequenceEnded{false} {

    // led sequence populated like 0-1-2-3-4-5-6-7-8-...
    ledSequence = new uint8_t[_PWIM_LED_NOTES_SIZE];
    for (uint8_t i = 0 ; i < _PWIM_LED_NOTES_SIZE ; i++) ledSequence[i] = i;

    // led pins are populated like PIN4-PIN5-PIN6-PIN4-PIN5-PIN6-PIN4-...
    this->ledPins = new uint8_t[_PWIM_LED_NOTES_SIZE];
    for (uint8_t i = 0 ; i < _PWIM_LED_NOTES_SIZE ; i++) this->ledPins[i] = ledPinsParam[i % ledPinsSize];
}

void mz::PlayWrongInputMelody::initialize(void(*sequenceEndCallback)()) {
    if (sequenceEndCallback != nullptr) {
        this->sequenceEndCallback = sequenceEndCallback;
    }

    initializeLedSequence(
        melodyBuzzer,
        ledPins,
        _PWIM_LED_NOTES_SIZE,
        _PWIM_LED_NOTES,
        ledSequence,
        _PWIM_LED_NOTES_SIZE,
        _PWIM_DURATIONS
    );
    
    sequenceEnded = false;
}

void mz::PlayWrongInputMelody::setup() {
    setupLedSequence();
    sequenceEnded = false;
}

void mz::PlayWrongInputMelody::update() {
    auto ledSequenceState = mz::parseLedSequenceState();

    switch (ledSequenceState) {
        case mz::LedSequenceState::RUNNING:
        case mz::LedSequenceState::READY:
            updateLedSequence();
            break;

        case mz::LedSequenceState::FINISHED:
            if(sequenceEndCallback != nullptr && !sequenceEnded) sequenceEndCallback();
            sequenceEnded = true;
            break;
        
        default:
            // do nothing...
            break;
    }
}

void mz::PlayWrongInputMelody::destroy() {
    destroyLedSequence();
}

mz::PlayWrongInputMelody::~PlayWrongInputMelody() {
    this->destroy();
    delete[] ledSequence;
    delete[] ledPins;
}
