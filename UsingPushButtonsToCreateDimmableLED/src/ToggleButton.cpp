#include "ToggleButton.h"

mz::ToggleButton::ToggleButton(void (*toggleAction)(int), int (*buttonSignalSupplier)()) : 
    toggleOn{false}, 
    pressCount{0}, 
    toggleAction{toggleAction},
    buttonSignalSupplier{buttonSignalSupplier} {};

void mz::ToggleButton::update() {
    // Reads signal from putton
    int buttonRead = this->buttonSignalSupplier();

    // Button was NOT being pressed and NOW it IS PRESSED
    if(!toggleOn && buttonRead) {
        toggleOn = true;
        pressCount++;
        this->toggleAction(pressCount);
    }

    // Button WAS pressed and NOW it is RELEASED
    if(toggleOn && !buttonRead) {
        toggleOn = false;
    }
}

int mz::ToggleButton::getPressCount() {
    return this->pressCount;
}
