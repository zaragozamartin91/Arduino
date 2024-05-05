#ifndef PLAYWRONGINPUTMELODY_H
#define PLAYWRONGINPUTMELODY_H

#include "MelodyBuzzer.h"

namespace mz {
    class PlayWrongInputMelody {
        private:
            mz::MelodyBuzzer* melodyBuzzer; // main buzzer
            uint8_t* ledPins; // AVAILABLE led pins array
            uint8_t ledPinsSize; // size of led pins array
            uint8_t* ledSequence; // array of game leds to play
            void(*sequenceEndCallback)(); // callback to invoke on sequence end

            bool playing; // melody playing
            bool ended; // melody ended

        public:
            PlayWrongInputMelody(
                mz::MelodyBuzzer* melodyBuzzer, // main buzzer
                uint8_t* ledPins, // led pins array ; ex {PIN4, PIN5, PIN6}
                uint8_t ledPinsSize, // size of led pins array'
                void(*sequenceEndCallback)() = nullptr
            );

            void initialize();

            void setup();

            void play(void(*sequenceEndCallback)() = nullptr);

            void update();

            void destroy();

            bool melodyPlaying();

            ~PlayWrongInputMelody();
        };
    
} // namespace mz


#endif