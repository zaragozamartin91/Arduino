#ifndef DEMOGAMESTATE_H
#define DEMOGAMESTATE_H

#include "ToggleButton.h"
#include "MelodyBuzzer.h"

namespace mz {
    class DemoGameState {
        private:
            ToggleButton* buttons;
            uint8_t buttonsSize;
            int* notes;
            MelodyBuzzer *melodyBuzzer;

            static unsigned long VERY_LONG_DELAY;

        public: 
            DemoGameState(ToggleButton* buttons, uint8_t buttonsSize, int* notes, MelodyBuzzer* melodyBuzzer);

            void update();
    };
}

#endif