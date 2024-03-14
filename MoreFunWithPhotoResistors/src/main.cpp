#include <Arduino.h>

const int POWER_PIN=PIN_A5;
const int MAX_POWER=1023;
const float MAX_TONE_DELAY=4000.;
const float POWER_FACTOR=(60. - MAX_TONE_DELAY) / static_cast<float>(MAX_POWER);
const int BUZZ_PIN=PIN5;

long v_currMicros = 0;
int v_powerRead = 0;
long v_buzzerToneDelay = 0;
long v_deltaMicros = 0;
long v_netBuzzerToneDelay = 0;

long parseBuzzerToneDelay(int powerRead);

void buzzAndDelay(int delayMicros);

void setup() {
  pinMode(POWER_PIN, INPUT);
  pinMode(BUZZ_PIN, OUTPUT);
  Serial.begin(9600);
  while(!Serial) {}
}

void loop() {
  v_currMicros = micros();

  v_powerRead = analogRead(POWER_PIN);

  // if power is 0    -> 10000 microsec delay
  // if power is 750 -> 60 microsec delay

  // Serial.print("POWER_FACTOR: "); Serial.println(POWER_FACTOR);

  v_buzzerToneDelay = parseBuzzerToneDelay(v_powerRead);
  v_deltaMicros = micros() - v_currMicros;
  v_netBuzzerToneDelay = max(v_buzzerToneDelay - v_deltaMicros, 0);

  // Serial.print("powerRead: "); Serial.println(powerRead);
  // Serial.print("buzzerToneDelay: "); Serial.println(buzzerToneDelay);
  // Serial.print("deltaMicros: "); Serial.println(deltaMicros);
  // Serial.print("netBuzzerToneDelay: "); Serial.println(netBuzzerToneDelay);

  // delay(1000);
  // buzzAndDelay(v_netBuzzerToneDelay);
}


long parseBuzzerToneDelay(int powerRead) {
  float toneDelay = MAX_TONE_DELAY + static_cast<float>(powerRead) * POWER_FACTOR;
  return static_cast<long>(toneDelay);
}

void buzzAndDelay(int delayMicros) {
  digitalWrite(BUZZ_PIN, HIGH);
  delayMicroseconds(delayMicros);
  digitalWrite(BUZZ_PIN, LOW);
  delayMicroseconds(delayMicros);
}