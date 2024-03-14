#include <Arduino.h>

int BLUE_KNOB_PIN = A5;
int GREEN_KNOB_PIN = A1;
int BLUE_OUTPUT_PIN = PIN3;
int GREEN_OUTPUT_PIN = 9;
char STR_BUFFER[256];

int mapRead(int read_value);

void setup() {
  pinMode(BLUE_KNOB_PIN, INPUT);
  pinMode(GREEN_KNOB_PIN, INPUT);
  pinMode(BLUE_OUTPUT_PIN, OUTPUT);

  Serial.begin(9600);
  while(!Serial) {}
}

void loop() {
  // put your main code here, to run repeatedly:
  int blue_read= analogRead(BLUE_KNOB_PIN);
  int green_read= analogRead(GREEN_KNOB_PIN);


  sprintf(STR_BUFFER, "blue_read: %d ; green_read: %d", blue_read, green_read);
  Serial.println(STR_BUFFER);

  analogWrite(BLUE_OUTPUT_PIN, mapRead(blue_read));
  analogWrite(GREEN_OUTPUT_PIN, mapRead(green_read));

  delay(500);
}

int mapRead(int read_value) {
  float mapped_value = 255. * static_cast<float>(read_value) / 1023.;
  return static_cast<int>(mapped_value);
}