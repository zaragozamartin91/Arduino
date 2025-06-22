#ifndef MOTOR_BUTTON_VALUE_H
#define MOTOR_BUTTON_VALUE_H

namespace mz {
    enum MotorButtonValue {
        MOTOR_BUTTON_NO_PRESS = 0,
        MOTOR_BUTTON_NO_PRESS_ = 1, // duplicate
        MOTOR_BUTTON_TOGGLE_OFF_SHORT_PRESS = 2,
        MOTOR_BUTTON_TOGGLE_OFF_LONG_PRESS = 3,
        MOTOR_BUTTON_TOGGLE_ON = 4,
        MOTOR_BUTTON_TOGGLE_ON_ = 5, // duplicate
        MOTOR_BUTTON_CONTINOUS_PRESS_SHORT = 6,
        MOTOR_BUTTON_CONTINOUS_PRESS_LONG = 7,
    };

    MotorButtonValue parseMotorButtonValue(
        bool buttonPressedCurrent, 
        bool motorButtonPressedPrevious, 
        bool longPress);
} // namespace mz



#endif