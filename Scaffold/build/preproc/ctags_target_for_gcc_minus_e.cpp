# 1 "C:\\Users\\martin\\Documents\\Workspaces\\Arduino\\Scaffold\\Scaffold.ino"
void setup()
{
    // put your setup code here, to run once:
    // ... --- ...
    pinMode(13, 0x1); // pin 13 is connected to board led
}

void loop()
{
    digitalWrite(13, 0x0);
    blink_long();
    blink_short();
}
# 1 "C:\\Users\\martin\\Documents\\Workspaces\\Arduino\\Scaffold\\Blink.ino"
void blink_short()
{
    digitalWrite(13, 0x1);
    delay(300);
    digitalWrite(13, 0x0);
    delay(300);
}

void blink_long()
{
    digitalWrite(13, 0x1);
    delay(1000);
    digitalWrite(13, 0x0);
    delay(300);
}
