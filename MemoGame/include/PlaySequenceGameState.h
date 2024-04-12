#ifndef PLAYSEQUENCEGAMESTATE_H
#define PLAYSEQUENCEGAMESTATE_H

#include "GameLed.h"


namespace mz {
    enum SequenceState {
        IDLE, PLAYING, END
    };

    class PlaySequenceGameState {
        private:
            void (*updateDelegate)(); // delegate update function
            GameLed* gameLeds; // game led array
            uint8_t gameLedCount; // count of game leds

            uint8_t* ledSequence; // sequence of leds to play
            uint8_t ledSequenceCount; // length of sequence
            
            uint16_t sequenceNoteLenght; // length of sequence note in milliseconds
            uint16_t sequenceDelay; // delay between sequence items in milliseconds

            SequenceState sequenceState;
            uint8_t ledSequenceIndex;

            void play();
            GameLed* currentGameLed(); // obtains current game led

        public: 
            PlaySequenceGameState(
                void (*updateDelegate)(), // delegate update function
                GameLed* ,// game led array
                uint8_t ,// count of game leds
                uint8_t* ,// sequence of leds to play
                uint8_t ,// length of sequence
                uint16_t ,// length of sequence note in milliseconds
                uint16_t// delay between sequence items in milliseconds
            );

            void update();
    };
}

#endif