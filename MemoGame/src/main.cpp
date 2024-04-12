#include <Arduino.h>
#include "music_notes.h" // including the library with the frequencies of the note 
#include "MelodyBuzzer.h"
#include "PlayLedSequence.h"
#include "MyMacros.h"

/* EXAMPLE code based on https://www.hackster.io/brambi001/play-any-song-with-arduino-passive-buzzer-000d52 written by Ilaria Brambilla - 25/09/2022 */

uint8_t BUZZER_PIN=PIN2;

uint8_t GREEN_BUTTON_PIN=PIN6;
uint8_t RED_BUTTON_PIN=9;
uint8_t BLUE_BUTTON_PIN=12;

uint8_t GREEN_LED_PIN = GREEN_BUTTON_PIN;
uint8_t RED_LED_PIN = RED_BUTTON_PIN;
uint8_t BLUE_LED_PIN = BLUE_BUTTON_PIN;

uint8_t LED_PINS[] = {GREEN_LED_PIN, RED_LED_PIN, BLUE_LED_PIN};
uint8_t LED_PINS_SIZE = sizeof(LED_PINS) / sizeof(LED_PINS[0]);

int GREEN_NOTE = NOTE_G4;
int RED_NOTE = NOTE_E4;
int BLUE_NOTE = NOTE_C4;

int LED_NOTES[] = {GREEN_NOTE, RED_NOTE, BLUE_NOTE};

mz::MelodyBuzzer melodyBuzzer(BUZZER_PIN, &tone, &noTone, &millis);


uint8_t ledSequence[] = {0, 1, 2, 1, 2, 0, 2, 1, 0};
uint8_t ledSequenceSize = sizeof(ledSequence) / sizeof(ledSequence[0]);
uint16_t ledSequenceDurations[] = {1000, 500, 1000, 250, 500, 1000, 300, 750, 250};


void setup () {
    pinMode(BUZZER_PIN, OUTPUT);

    Serial.begin(9600);
    while(!Serial){};

    debug("ledSequenceSize: ", ledSequenceSize);

    debug("Initializing led sequence", ".");
    mz::initializeLedSequence(
      &melodyBuzzer, LED_PINS, LED_PINS_SIZE, LED_NOTES, ledSequence, ledSequenceSize, ledSequenceDurations
    );

    debug("Setting up led sequence", ".");
    mz::setupLedSequence();
}


int LOOP_DELAY_TIME = 100;

void loop() {

  if (mz::ledSequenceDone()) {
    debug("Destroying led sequence",".");
    mz::destroyLedSequence();
    debug("Led sequence destroyed",".");
  } else {
    debug("Led sequence index: ", mz::getLedSequenceIndex());
    mz::updateLedSequence();
  }

  melodyBuzzer.update();
  delay(LOOP_DELAY_TIME);
}
