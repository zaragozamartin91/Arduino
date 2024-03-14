#include <Arduino.h>

// From video tutorial https://www.youtube.com/watch?v=5TitZmA66bI&list=PLGs0VKk2DiYw-L-RibttcvK-WBZm8WLEP&index=10

int readPin = PIN_A3;
long delayTimeMilisecods = 500;

// put function declarations here:
float asVolts(float);

void setup() {
  
  // The default monitor speed is configured in the platform.ini file using the monitor_speed key
  // For more info on baud rate, visit https://www.youtube.com/watch?v=8GX5brSZ_1E
  Serial.begin(115200); // The baud rate defines how quick will the port read and write data
  while(!Serial) {};
  Serial.println("Serial comm started");
  
  pinMode(readPin, INPUT);
}

void loop() {
  // Serial.println("Reading analog signal");
  // put your main code here, to run repeatedly:
  float analogReadValue = analogRead(readPin);
  String msg0("analogReadValue = ");
  msg0.concat(String(analogReadValue, 2));
  
  Serial.println(msg0);

  float voltage = asVolts(analogReadValue);
  String msg1 = "voltage = ";
  Serial.print(msg1);
  Serial.println(voltage);

  delay(500);
}

// put function definitions here:
float asVolts(float analogRead) {
  // analogRead yields values between 0 and 1023
  // max read value represents max voltage (in this case, 5v)
  // using leading . notation to use float constants
  return 5. * analogRead / 1023.;
}