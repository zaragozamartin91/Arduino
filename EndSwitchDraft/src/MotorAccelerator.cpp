#include "MotorAccelerator.h"

namespace mz {
    
    MotorAccelerator::MotorAccelerator(
        unsigned long (*getTimeMillis)(),
        L298N *motor,
        unsigned short startSpeed,
        unsigned short finalSpeed,
        unsigned long timeMs,
        int partition) {
            
            this->getTimeMillis=getTimeMillis; // function for obtaining current time in milliseconds
            this->motor = motor;
            this->startSpeed = startSpeed;
            this->finalSpeed = finalSpeed;
            this->timeMs = timeMs;
            this->partition = partition;

        }

    MotorAccelerator::~MotorAccelerator() {
    }

    MotorAccelerator *MotorAccelerator::start() {
        timeDiv = div(timeMs, partition);
        speedDiv = div(finalSpeed, partition);
        startTime = getTimeMillis();
        endTime = startTime + timeMs;
        motor->setSpeed(startSpeed);

        return this;
    }

    void accelerate(
        L298N* mtr,
        unsigned short startSpeed,
        unsigned short finalSpeed,
        unsigned long timeMs,
        int partition = 10) {
      
        div_t timeDiv = div(timeMs, partition);
        div_t speedDiv = div(finalSpeed, partition);
      
        mtr->setSpeed(startSpeed);
        for (int i = 0 ; i < partition && timeDiv.quot > 0 ; i++) {
          delay(timeDiv.quot);
          mtr->setSpeed( mtr->getSpeed() + speedDiv.quot );
        }
      
        if (timeDiv.rem > 0) {
          delay(timeDiv.rem);
          mtr->setSpeed(finalSpeed);
        }
      }
}
