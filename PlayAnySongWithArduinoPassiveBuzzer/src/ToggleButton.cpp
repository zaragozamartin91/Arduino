#include "ToggleButton.h"

void safeCall(void (*callback)(int), int arg) {
    if(callback != nullptr) { callback(arg); }
}


mz::ToggleButton::ToggleButton(
    int (*buttonSignalSupplier)(), 
    void (*toggleCallback)(int),
    void (*pressCallback)(int),
    void (*releaseCallback)(int)) : 
    
    toggleOn{false}, 
    pressCount{0}, 
    buttonRead{0},
    buttonSignalSupplier{buttonSignalSupplier},
    toggleCallback{toggleCallback},
    pressCallback{pressCallback},
    releaseCallback{releaseCallback} {};

void mz::ToggleButton::update() {
    // Reads signal from putton
    buttonRead = this->buttonSignalSupplier();

    // Button was NOT being pressed and NOW it IS PRESSED
    if(!toggleOn && buttonRead) {
        toggleOn = true;
        pressCount++;
        safeCall(toggleCallback, pressCount);
    }

    // Button WAS pressed and NOW it is RELEASED
    if(toggleOn && !buttonRead) { toggleOn = false; }


    if (buttonRead) { safeCall(pressCallback, pressCount); } 
    else            { safeCall(releaseCallback, pressCount);}
}

int mz::ToggleButton::getPressCount() {
    return this->pressCount;
}

int mz::ToggleButton::buttonPressed() {
    return buttonRead;
}
