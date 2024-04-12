#include "DemoGameState.h"
#include "Arduino.h"



// Declaring static variables https://en.cppreference.com/w/cpp/language/static
unsigned long mz::DemoGameState::VERY_LONG_DELAY = 3600000;

mz::DemoGameState::DemoGameState(ToggleButton *buttons, uint8_t buttonsSize, int *notes, MelodyBuzzer *melodyBuzzer) {
    this->buttonsSize = buttonsSize;
    this->buttons = buttons;
    this->notes = notes;
    this->melodyBuzzer = melodyBuzzer;
}

// void onMelodyEnd(unsigned int, unsigned long,unsigned long,unsigned long) {
//     Serial.println("Melody end!");
// }

void mz::DemoGameState::update() {
    int pressedButton = -1;
    for (int i = 0; i < 3; i++) {
        buttons[i].update();
        if(buttons[i].buttonPressed()) pressedButton = i;
    }

    // declaring lambda functions: https://www.youtube.com/watch?v=mWgmBBz0y8c
    void(*onMelodyEnd)(unsigned int, unsigned long,unsigned long,unsigned long)=
        [](unsigned int x, unsigned long y ,unsigned long z,unsigned long a) {
            Serial.println("Melody end!");
        };

    if(pressedButton >= 0) {
        melodyBuzzer->playAsync(notes[pressedButton], VERY_LONG_DELAY, onMelodyEnd);
    } else {
        melodyBuzzer->stop();    
    }

    melodyBuzzer->update();
}
