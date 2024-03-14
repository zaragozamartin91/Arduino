#include <Arduino.h>
#include "EasyBuzzer.h"


unsigned int frequency = 900;  
unsigned int onDuration = 50;
unsigned int offDuration = 100;
unsigned int beeps = 2;
unsigned int pauseDuration = 500;
unsigned int cycles = 10;

const int BUZZER_PIN = PIN5;

void done() {
	Serial.print("Done!");
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);

	Serial.begin(9600);
  while(!Serial){}

  Serial.println("Begin beeping");

	/* Start a beeping sequence. */
	EasyBuzzer.beep(
		frequency,		// Frequency in hertz(HZ). 
		onDuration, 	// On Duration in milliseconds(ms).
		offDuration, 	// Off Duration in milliseconds(ms).
		beeps, 			// The number of beeps per cycle.
		pauseDuration, 	// Pause duration.
		cycles, 		// The number of cycle.
		done			// [Optional] Callback. A function to call when the sequence ends.
	);

  EasyBuzzer.setPin(BUZZER_PIN);
}

void loop() {

	/* Always call this function in the loop for EasyBuzzer to work. */
	EasyBuzzer.update();
}
