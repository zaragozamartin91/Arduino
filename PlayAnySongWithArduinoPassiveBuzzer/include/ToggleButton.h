#ifndef TOGGLEBTN_H
#define TOGGLEBTN_H

namespace mz
{
    class ToggleButton {
        private:
            bool toggleOn; // keeps track of toggling
            int pressCount; // counts total press count
            int buttonRead; // signal value
            int (*buttonSignalSupplier)(); // Supplier of the button signal
            void (*toggleCallback)(int); // Action to execute when toggle happens
            void (*pressCallback)(int); // Action to execute while button is being pressed
            void (*releaseCallback)(int); // Action to execute while button is released

        public:
            ToggleButton(
                int (*buttonSignalSupplier)(), 
                void (*toggleCallback)(int) = nullptr, 
                void (*pressCallback)(int) = nullptr,
                void (*releaseCallback)(int) = nullptr
            );

            /* To be called on every loop cycle. @param buttonRead Value of button read. */
            void update();

            // returns press count
            int getPressCount();

            // returns current value of button signal
            int buttonPressed();
    };
} // namespace mz

#endif
