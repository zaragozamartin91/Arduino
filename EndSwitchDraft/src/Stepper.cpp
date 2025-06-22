#include "Stepper.h"
#include <stdlib.h>

mz::Stepper::Stepper(
    unsigned long (*getTimeMillis)(),
    void (*action)(),
    unsigned long periodMs,
    unsigned int iterations) {

    this->getTimeMillis = getTimeMillis;
    this->action = action;
    this->periodMs = periodMs;
    this->iterations = iterations;

    iterationCount = 0;
}

mz::Stepper::~Stepper(){
}

mz::Stepper* mz::Stepper::setup(){
    unsigned long currTimeMs = getTimeMillis();
    nextStepMs = currTimeMs + periodMs;
    return this;
}

mz::Stepper* mz::Stepper::loop() {
    // iterations exhausted
    if (exhausted()) { return this; }

    unsigned long currTimeMs = getTimeMillis();
    long timeDiffMs = currTimeMs - nextStepMs;

    // wait some more...
    if (timeDiffMs < 0) { return this; }

    auto executions = div(timeDiffMs, periodMs);

    // wait some more...
    if (executions.quot <= 0) { return this; }

    // Execute action N times
    for (int i = 0 ; i < executions.quot && !exhausted() ; i++ , iterationCount++) { 
        this->action();
    }

    nextStepMs = nextStepMs + periodMs * executions.quot;

    return this;
}

unsigned int mz::Stepper::remainingIterations() {
    return iterationCount >= iterations 
    ? 0 
    : iterations - iterationCount;
}

bool mz::Stepper::exhausted() {
    return remainingIterations() == 0;
}

mz::Stepper* mz::Stepper::reset() {
    iterationCount = 0;
    return this;
}
