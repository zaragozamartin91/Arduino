#include "ToggleButton.h"

mz::ToggleButton::ToggleButton(void (*toggleAction)(int)) : toggleOn{false}, pressCount{0} , toggleAction{toggleAction} {};

void mz::ToggleButton::update(int buttonRead) {
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
