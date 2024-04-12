#include "PlaySequenceGameState.h"
#include "GameLed.h"


mz::GameLed* mz::PlaySequenceGameState::currentGameLed() {
    uint8_t ledIndex = ledSequence[ledSequenceIndex];
    return &gameLeds[ledIndex];
}

void mz::PlaySequenceGameState::play() {
    GameLed* gameLed = this->currentGameLed();
    gameLed->turnOn(this->sequenceNoteLenght);
    this->sequenceState = PLAYING;
}

mz::PlaySequenceGameState::PlaySequenceGameState(
    void (*updateDelegate)(),    // delegate update function
    GameLed *gameLeds,           // game led array
    uint8_t gameLedCount,        // count of game leds
    uint8_t *ledSequence,        // sequence of leds to play
    uint8_t ledSequenceCount,    // length of sequence
    uint16_t sequenceNoteLenght, // length of sequence note in milliseconds
    uint16_t sequenceDelay       // delay between sequence items in milliseconds
)
{
    this->updateDelegate = updateDelegate;
    this->gameLeds = gameLeds;
    this->gameLedCount = gameLedCount;
    this->ledSequence = ledSequence;
    this->ledSequenceCount = ledSequenceCount;
    this->sequenceNoteLenght = sequenceNoteLenght;
    this->sequenceDelay = sequenceDelay;
    this->sequenceState = IDLE;
    this->ledSequenceIndex = 0;
}

void mz::PlaySequenceGameState::update() {
    switch (sequenceState) {
        case IDLE:
            // start sequence
            this->play();
            break;

        case END:
            // nothing to do here
            break;
        
        case PLAYING:
            this->updateDelegate();
            
            break;

        default:
            break;
    }
}

