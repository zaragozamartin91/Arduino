#include <Arduino.h>

// video https://www.youtube.com/watch?v=SYDuRyweeLA&list=PLGs0VKk2DiYw-L-RibttcvK-WBZm8WLEP&index=36

#define _TILT_PIN 2
#define _GREEN_PIN 8
#define _RED_PIN 11

void setup() {
  pinMode(_TILT_PIN, INPUT);
  /* Using the pin as a pull-up resistor */
  /* Watch https://www.youtube.com/watch?v=ChHNI8yt69g&list=PLGs0VKk2DiYw-L-RibttcvK-WBZm8WLEP&index=34&pp=iAQB for more info */
  /* The same results can be achieved by doing `pinMode(_TILT_PIN, INPUT_PULLUP);` */
  digitalWrite(_TILT_PIN, HIGH);

  pinMode(_GREEN_PIN, OUTPUT);
  pinMode(_RED_PIN, OUTPUT);

  Serial.begin(9600);
  while(!Serial) {};
}

#define _LOOP_PERIOD 500
void loop() {
  auto tiltValue = digitalRead(_TILT_PIN);

  auto redPinOn = tiltValue >= 1;
  auto greenPinOn = !redPinOn;
  digitalWrite(_GREEN_PIN, greenPinOn);
  digitalWrite(_RED_PIN, redPinOn);

  delay(_LOOP_PERIOD);
}


