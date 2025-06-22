#ifndef MotorControlState_h
#define MotorControlState_h

/* Prefix included to the enum since its plain */
/* More information here https://stackoverflow.com/questions/18335861/why-is-enum-class-considered-safer-to-use-than-plain-enum */
namespace mz {
    enum MotorControlState {
        MOTOR_CONTROL_STARTING,
        MOTOR_CONTROL_MOVING,
        MOTOR_CONTROL_HALTING,
        MOTOR_CONTROL_IDLE
    };

    MotorControlState parseMotorControlState(bool moveMotor, bool motorIsMoving);
}

#endif
