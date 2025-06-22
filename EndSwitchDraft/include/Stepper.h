#ifndef Stepper_h
#define Stepper_h

namespace mz {
    class Stepper {
        private:
            unsigned long (*getTimeMillis)(); // function for obtaining current time in milliseconds
            void (*action)(); // action to execute on every step

            unsigned long periodMs; // Execute action every periodMs milliseconds
            unsigned int iterations; // number of times to step

            unsigned long nextStepMs; // moment to execute next step

            unsigned int remainingIterations();
            unsigned int iterationCount;
            bool exhausted();
        public:
            Stepper(
                unsigned long (*getTimeMillis)(), // function for obtaining current time in milliseconds
                void (*action)(), // action to execute on every step or loop
                unsigned long periodMs, // Execute action every periodMs milliseconds
                unsigned int iterations // number of times to step
            );
            ~Stepper();

            Stepper* setup();

            Stepper* loop();
        
            Stepper* reset();
    };

    /* This stepper is always exhausted */
    // Stepper NO_OP_STEPPER(
    //     [](){ unsigned long res = 0; return res; },
    //     [](){ },
    //     0,
    //     0
    // );
}

#endif