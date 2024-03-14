void setup() {
  // put your setup code here, to run once:
  pinMode(13,OUTPUT); // pin 13 is connected to board led
  Serial.begin(9600);
  while (!Serial);  // Wait for serial port to connect
  Serial.println("Serial com started!");
}

void loop() {
  // put your main code here, to run repeatedly:
  // digitalWrite(13, HIGH);
  //delay(900);
  digitalWrite(13, LOW);
  Serial.println("Hello world!");
  delay(2000);
  //delay(100);
}
