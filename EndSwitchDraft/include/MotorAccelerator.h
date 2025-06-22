#ifndef MotorAccelerator_h
#define MotorAccelerator_h

#include <L298N.h>

namespace mz {
    class MotorAccelerator {
        private:
            unsigned long (*getTimeMillis)(); // function for obtaining current time in milliseconds
            L298N* motor;
            unsigned short startSpeed;
            unsigned short finalSpeed;
            unsigned long timeMs;
            int partition = 10;

            div_t timeDiv;
            div_t speedDiv;
            unsigned long startTime;
            unsigned long endTime;
        public:
            MotorAccelerator(
                unsigned long (*getTimeMillis)(), // function for obtaining current time in milliseconds
                L298N* motor,
                unsigned short startSpeed,
                unsigned short finalSpeed,
                unsigned long timeMs,
                int partition = 10
            );
            ~MotorAccelerator();
        
            MotorAccelerator* start();
    };
}

#endif