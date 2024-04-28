#ifndef GAMEBUTTON_H
#define GAMEBUTTON_H

#include <stdint.h>
#include "ToggleButton.h"

namespace mz {
    class GameButton {
        private:
            uint8_t pin;
            void(*setupFunction)(uint8_t); // setup function
            ToggleButton toggleButton;
            bool setupDone; // indicates if setup has been executed

        public:
            GameButton(
                uint8_t pin, // button pin
                void(*setupFunction)(uint8_t), // setup function
                int(*buttonSignalSupplier)(int),  // supplier of button signal
                void(*toggleCallback)(int)  // function to invoke on toggle
            );

            GameButton(); // default constructor used for array initialization

            void setup();

            void update();

            void reset();
    };
} // namespace mz


#endif