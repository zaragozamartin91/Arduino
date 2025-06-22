#include "MotorControlState.h"

mz::MotorControlState mz::parseMotorControlState(bool moveMotor, bool motorIsMoving) {
    if (moveMotor && motorIsMoving)   return mz::MotorControlState::MOTOR_CONTROL_MOVING;
    if (!moveMotor && motorIsMoving)  return mz::MotorControlState::MOTOR_CONTROL_HALTING;
    if (moveMotor && !motorIsMoving)  return mz::MotorControlState::MOTOR_CONTROL_STARTING;
    /* else */                        return mz::MotorControlState::MOTOR_CONTROL_IDLE;
}
