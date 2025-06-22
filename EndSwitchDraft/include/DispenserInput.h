#ifndef DispenserInput_h
#define DispenserInput_h

namespace mz{
    class DispenserInput {

        private:
            bool endSwitchPressed;
            bool motorButtonPressed;
        public:
            DispenserInput(/* args */);
            ~DispenserInput();

            // Constructor with parameters
            DispenserInput(bool endSwitchPressed, bool motorButtonPressed);

            // assignment operator
            DispenserInput& operator=(const DispenserInput& other);

            // Copy constructor
            DispenserInput(const DispenserInput& other);

            // Getters
            bool isEndSwitchPressed() const;
            bool isMotorButtonPressed() const;
        };
} // namespace mz

#endif