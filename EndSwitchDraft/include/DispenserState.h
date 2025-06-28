#ifndef DispenserState_h
#define DispenserState_h

/* Prefix included to the enum since its plain */
/* More information here https://stackoverflow.com/questions/18335861/why-is-enum-class-considered-safer-to-use-than-plain-enum */
namespace mz {
    enum DispenserState {
        DISPENSER_IDLE,
        DISPENSER_FORCE_MOVE_MOTOR,
        DISPENSER_COUNTDOWN,
        DISPENSER_MOVE_MOTOR_UNTIL_END_SWITCH_PRESSED,
    };
}

#endif