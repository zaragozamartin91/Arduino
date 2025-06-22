#include "DispenserInput.h"

namespace mz {

    DispenserInput::DispenserInput() {
    }
    
    DispenserInput::~DispenserInput(){
    }
    
    DispenserInput::DispenserInput(bool endSwitchPressed, bool motorButtonPressed)
        : endSwitchPressed(endSwitchPressed), motorButtonPressed(motorButtonPressed) {}
    
    DispenserInput::DispenserInput(const DispenserInput& other) : 
        endSwitchPressed(other.endSwitchPressed), 
        motorButtonPressed(other.motorButtonPressed) {}
    
     DispenserInput& DispenserInput::operator=(const DispenserInput& other) {
        endSwitchPressed = other.endSwitchPressed;
        motorButtonPressed = other.motorButtonPressed;
        return *this;
    }

    bool DispenserInput::isEndSwitchPressed() const { return endSwitchPressed; }
    bool DispenserInput::isMotorButtonPressed() const { return motorButtonPressed; }
} // namespace mz
