#include <Arduino.h>

const int POWER_PIN=PIN_A5;
const int MAX_POWER=1023;
const float POWER_FACTOR=(60. - 10000.) / static_cast<float>(MAX_POWER);
const int BUZZ_PIN=PIN5;

int parseBuzzerToneDelay(int powerRead);

void buzzAndDelay(int delayMicros);

void setup() {
  pinMode(POWER_PIN, INPUT);
  pinMode(BUZZ_PIN, OUTPUT);
  Serial.begin(9600);
  while(!Serial) {}
}

void loop() {
  int powerRead = analogRead(POWER_PIN);
  // Serial.print("powerRead: "); Serial.println(powerRead);

  // if power is 0    -> 10000 microsec delay
  // if power is 1023 -> 60 microsec delay

  // Serial.print("POWER_FACTOR: "); Serial.println(POWER_FACTOR);

  int buzzerToneDelay = parseBuzzerToneDelay(powerRead);
  // Serial.print("buzzerToneDelay: "); Serial.println(buzzerToneDelay);

  // delay(1000);
  // buzzAndDelay(buzzerToneDelay);
}


int parseBuzzerToneDelay(int powerRead) {
  float toneDelay = 10000. + static_cast<float>(powerRead) * POWER_FACTOR;
  return static_cast<int>(toneDelay);
}

void buzzAndDelay(int delayMicros) {
  digitalWrite(BUZZ_PIN, HIGH);
  delayMicroseconds(delayMicros);
  digitalWrite(BUZZ_PIN, LOW);
  delayMicroseconds(delayMicros);
}