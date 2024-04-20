#ifndef GAMELED_H
#define GAMELED_H

#include <stdint.h>

namespace mz {

    class GameLed {
        private:
            uint8_t pin;
            void (*setupFunction)(uint8_t); // setup function
            void (*turnOnLed)(uint8_t);
            void (*turnOffLed)(uint8_t);
            void (*playNote)(unsigned int frequency, unsigned long duration); // play note on buzzer function
            int note; // default note to play
            bool setupDone;

            static unsigned long VERY_LONG_DELAY;
            static int DEFAULT_NOTE;

        public:
            GameLed(
                uint8_t pin,
                void (*setupFunction)(uint8_t), // setup function ; eg: pinMode(PIN3, OUTPUT)
                void (*turnOnLed)(uint8_t),
                void (*turnOffLed)(uint8_t),
                void (*playNote)(unsigned int frequency, unsigned long duration), // play note on buzzer function
                int note // default note to play
            );

            /* 
            Default constructor used for array initialization. Do not invoke directly.
            https://stackoverflow.com/questions/8462895/how-to-dynamically-declare-an-array-of-objects-with-a-constructor-in-c 
            */
            GameLed();

            /* 
            Copy constructor.
            Is invoked when assignation operator is used.
            https://en.cppreference.com/w/cpp/language/copy_constructor
             */
            GameLed(GameLed& other);

            void setup();

            void turnOn(
                unsigned long duration = VERY_LONG_DELAY,
                int noteOverride = DEFAULT_NOTE // override note
            );

            void turnOff();

            void reset();
    };
}

#endif