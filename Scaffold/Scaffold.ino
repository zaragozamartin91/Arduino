void setup()
{
    // put your setup code here, to run once:
    // ... --- ...
    pinMode(13, OUTPUT); // pin 13 is connected to board led
}

void loop()
{
    digitalWrite(13, LOW);
    blink_long();
    blink_short();
}
