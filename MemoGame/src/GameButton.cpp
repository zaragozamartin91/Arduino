#include "GameButton.h"


mz::GameButton::GameButton(

    uint8_t pin, 
    void (*setupFunction)(uint8_t), 
    int (*buttonSignalSupplier)(int), 
    void (*toggleCallback)(int)
    
    ) : pin{pin} , setupFunction{setupFunction} , setupDone{false} {

    ToggleButton tb(pin, buttonSignalSupplier, toggleCallback);
    this->toggleButton = tb; // copy constructor invoked
}

mz::GameButton::GameButton() : pin{255}, setupFunction{nullptr}, setupDone{false} {}

mz::GameButton::GameButton(GameButton &other) {
    pin = other.pin;
    setupFunction = other.setupFunction; // setup function
    toggleButton = other.toggleButton;
    setupDone = other.setupDone; // indicates if setup has been executed
}

void mz::GameButton::setup() {
    if(this->setupDone) {return;} // nothing to do here
    
    setupFunction(pin); // run setup
    setupDone = true;
}

void mz::GameButton::update() {
    this->toggleButton.update();
}

void mz::GameButton::reset() {
    this->setupDone = false;
}
