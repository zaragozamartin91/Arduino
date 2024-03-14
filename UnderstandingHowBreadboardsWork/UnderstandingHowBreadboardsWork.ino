// Video: https://www.youtube.com/watch?v=CfdaJ4z4u4w&list=PLGs0VKk2DiYw-L-RibttcvK-WBZm8WLEP&index=3

uint8_t quick_pin = 7;
uint8_t mid_pin = 5;
uint8_t slow_pin = 3;

unsigned long blink_length = 500;
unsigned long cycle_lenght = blink_length / 2;
unsigned long cycle_count = 0;

unsigned long quick_pin_blinks = 2; // blink 5 times within the period]
unsigned long mid_pin_blinks = 4; // blink 4 times within the period
unsigned long slow_pin_blinks = 6; // blink 2 times within the period

void setup()
{
    pinMode(quick_pin, OUTPUT);
    pinMode(mid_pin, OUTPUT);
    pinMode(slow_pin, OUTPUT);

    // Using serial monitor to debug https://www.makerguides.com/how-to-print-to-serial-monitor-on-arduino/
    Serial.begin(9600);
    while (!Serial)
        ; // Wait for serial port to connect
    Serial.println("Serial com started!");
}

void do_nothing() {}

void loop()
{
    // main_program();
    do_nothing();
}

void debug_serial_port_example()
{
    int ENOUGH = (int)((ceil(log10(42)) + 1) * sizeof(char));
    char str[ENOUGH];
    sprintf(str, "%d", 42);
    Serial.println(str);
}

void main_program()
{
    unsigned long elapsed_time = cycle_count * cycle_lenght;

    uint8_t quick_pin_on = cycle_count > 0 && cycle_count % quick_pin_blinks == 0;
    digitalWrite(quick_pin, quick_pin_on);

    uint8_t mid_pin_on = cycle_count > 0 && cycle_count % mid_pin_blinks == 0;
    digitalWrite(mid_pin, mid_pin_on);

    uint8_t slow_pin_on = cycle_count > 0 && cycle_count % slow_pin_blinks == 0;
    digitalWrite(slow_pin, slow_pin_on);

    delay(cycle_lenght);
    cycle_count++;

    if (cycle_count > 100000)
        cycle_count = 0; // reset cycle count just in case
}
