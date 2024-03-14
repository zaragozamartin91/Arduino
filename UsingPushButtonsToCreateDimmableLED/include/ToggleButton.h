#ifndef TOGGLEBTN_H
#define TOGGLEBTN_H

namespace mz
{
    class ToggleButton {
        private:
            bool toggleOn; // keeps track of toggling
            int pressCount;
            void (*toggleAction)(int); // Action to apply when toggle happens
            int (*buttonSignalSupplier)(); // Supplier of the button signal


        public:
            ToggleButton(void (*toggleAction)(int), int (*buttonSignalSupplier)());

            /* To be called on every loop cycle. @param buttonRead Value of button read. */
            void update();

            int getPressCount();
    };
} // namespace mz

#endif
