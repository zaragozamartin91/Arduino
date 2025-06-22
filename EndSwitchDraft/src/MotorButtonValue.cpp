#include "MotorButtonValue.h"

mz::MotorButtonValue mz::parseMotorButtonValue(bool buttonPressedCurrent, bool motorButtonPressedPrevious, bool longPress) {
    short index = 
    (buttonPressedCurrent << 2)         // 4
    | (motorButtonPressedPrevious << 1) // 2 
    | longPress;                        // 1
    return static_cast<MotorButtonValue>(index);
}
