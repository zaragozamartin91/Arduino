#include "GameLed.h"

unsigned long mz::GameLed::VERY_LONG_DELAY = 3600000;
int mz::GameLed::DEFAULT_NOTE = -1;

mz::GameLed::GameLed(
                uint8_t pin,
                void (*setupFunction)(uint8_t), // setup function ; eg: pinMode(PIN3, OUTPUT)
                void (*turnOnLed)(uint8_t),
                void (*turnOffLed)(uint8_t),
                void (*playNote)(unsigned int frequency, unsigned long duration), // play note on buzzer function
                int note // default note to play
            ) {
    this->pin = pin;
    this->setupFunction = setupFunction;
    this->turnOnLed = turnOnLed;
    this->turnOffLed = turnOffLed;
    this->playNote = playNote;
    this->note = note;
    this->setupDone = false;
}

void mz::GameLed::setup() { 
    if (setupDone) return ; // nothing to do here
    this->setupFunction(pin);
    this->setupDone = true;
}


void mz::GameLed::turnOn(unsigned long duration, int noteOverride) {
    this->turnOnLed(pin);

    int n = noteOverride == DEFAULT_NOTE ? this->note : noteOverride;
    this->playNote(n, duration);
}


void mz::GameLed::turnOff() {
    this->turnOffLed(pin);
}


void mz::GameLed::reset() { this->setupDone=false; }


mz::GameLed::GameLed() {
    this->pin = -1;
    this->setupFunction = nullptr;
    this->turnOnLed = nullptr;
    this->turnOffLed = nullptr;
    this->playNote = nullptr;
    this->note = 0;
    this->setupDone = false;
};

mz::GameLed::GameLed(GameLed& other) {
    this->pin = other.pin;
    this->setupFunction = other.setupFunction;
    this->turnOnLed = other.turnOnLed;
    this->turnOffLed = other.turnOffLed;
    this->playNote = other.playNote;
    this->note = other.note;
    this->setupDone = false;
}