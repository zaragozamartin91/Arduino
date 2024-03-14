#include <Arduino.h>
#line 1 "C:\\Users\\martin\\Documents\\Workspaces\\Arduino\\Scaffold\\Scaffold.ino"
#line 1 "C:\\Users\\martin\\Documents\\Workspaces\\Arduino\\Scaffold\\Scaffold.ino"
void setup();
#line 8 "C:\\Users\\martin\\Documents\\Workspaces\\Arduino\\Scaffold\\Scaffold.ino"
void loop();
#line 1 "C:\\Users\\martin\\Documents\\Workspaces\\Arduino\\Scaffold\\Blink.ino"
void blink_short();
#line 9 "C:\\Users\\martin\\Documents\\Workspaces\\Arduino\\Scaffold\\Blink.ino"
void blink_long();
#line 1 "C:\\Users\\martin\\Documents\\Workspaces\\Arduino\\Scaffold\\Scaffold.ino"
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

#line 1 "C:\\Users\\martin\\Documents\\Workspaces\\Arduino\\Scaffold\\Blink.ino"
void blink_short()
{
    digitalWrite(13, HIGH);
    delay(300);
    digitalWrite(13, LOW);
    delay(300);
}

void blink_long()
{
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(300);
}
