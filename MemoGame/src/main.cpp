#include <Arduino.h>
#include "music_notes.h" // including the library with the frequencies of the note 
#include "MelodyBuzzer.h"
#include "ToggleButton.h"

/* EXAMPLE code based on https://www.hackster.io/brambi001/play-any-song-with-arduino-passive-buzzer-000d52 written by Ilaria Brambilla - 25/09/2022 */

int BUZZER_PIN=PIN2;
int GREEN_BUTTON=PIN4;
int BLUE_BUTTON=8;
int RED_BUTTON=12;
mz::MelodyBuzzer melodyBuzzer(BUZZER_PIN, &tone, &noTone, &millis);

int melody[] = {
  NOTE_C4, NOTE_A3, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_AS3, NOTE_F4, NOTE_F4, NOTE_E4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_AS3, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4
}; // declaring the notes of the melody (they change depending on the song you wanna play)

int melodySize = sizeof(melody) / sizeof(melody[0]);

int durations[] = {
  2, 2, 3, 3, 3, 2, 3, 3, 2, 1, 3, 3, 3, 5, 5, 5, 1, 2, 1, 3, 3, 3, 3, 1
}; // declaring the duration of each note (4 is a quarter note ecc)

mz::MelodyBuzzer buzzers[] = {
  mz::MelodyBuzzer(BUZZER_PIN, &tone, &noTone, &millis)
};

void setup () {
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(GREEN_BUTTON, INPUT);
    pinMode(BLUE_BUTTON, INPUT);
    pinMode(RED_BUTTON, INPUT);
    Serial.begin(9600);
    while(!Serial){};
}


int melodyPtr = 0;

int calculateDurationMilliseconds(int durationVal) { return 1000 / durations[melodyPtr]; }

void onTuneEnd(unsigned int frequency, unsigned long startTime, unsigned long endTime, unsigned long currentTime) {
  Serial.print("frequency: "); Serial.print(frequency);
  Serial.print(" ; startTime: "); Serial.print(startTime);
  Serial.print(" ; endTime: "); Serial.print(endTime);
  Serial.print(" ; currentTime: "); Serial.print(currentTime);
  Serial.println();

  int duration = calculateDurationMilliseconds(durations[melodyPtr]);
  int pause = duration * 0.25; // adding a little delay in between each note to separate the sounds
  delay(pause);

  ++melodyPtr;
}


int LOOP_DELAY_TIME = 50;
unsigned long VERY_LONG_DELAY = 3600000;

int greenButtonPressed() {return digitalRead(GREEN_BUTTON);}
int blueButtonPressed() {return digitalRead(BLUE_BUTTON);}
int redButtonPressed() {return digitalRead(RED_BUTTON);}

void onRedToggle(int _) { Serial.println("Red button pressed!"); }

mz::ToggleButton greenButton(&greenButtonPressed);
mz::ToggleButton blueButton(&blueButtonPressed);
mz::ToggleButton redButton(&redButtonPressed, &onRedToggle);

void loop() {
  greenButton.update();
  blueButton.update();
  redButton.update();

  if (greenButton.buttonPressed()) {
    melodyBuzzer.playAsync(NOTE_G4, VERY_LONG_DELAY);    
  } else if (blueButton.buttonPressed()) {
    melodyBuzzer.playAsync(NOTE_E4, VERY_LONG_DELAY);    
  } else if (redButton.buttonPressed()) {
    melodyBuzzer.playAsync(NOTE_C4, VERY_LONG_DELAY);    
  } else {
    melodyBuzzer.stop();
  }

  melodyBuzzer.update();

  delay(LOOP_DELAY_TIME);
}

