#ifndef TOGGLEBTN_H
#define TOGGLEBTN_H

namespace mz
{
    class ToggleButton {
        private:
            bool toggleOn; // keeps track of toggling
            int pressCount;
            void (*toggleAction)(int); // Action to apply when toggle happens


        public:
            ToggleButton(void (*toggleAction)(int));

            /* To be called on every loop cycle. @param buttonRead Value of button read. */
            void update(int buttonRead);

            int getPressCount();
    };
} // namespace mz

#endif
