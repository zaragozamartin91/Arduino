#include <Arduino.h>

int RED_PIN = 12;
int GREEN_PIN = 7;
int BLUE_PIN = 3;

String RED_STR = "RED";
String GREEN_STR = "GREEN";
String BLUE_STR = "BLUE";
String AQUA_STR = "AQUA";
String FUSCIA_STR = "FUSCIA";
String OFF_STR = "OFF";
String VALID_INPUTS[6] = {RED_STR, GREEN_STR, BLUE_STR, AQUA_STR, FUSCIA_STR, OFF_STR};
size_t VALID_INPUTS_SIZE = sizeof VALID_INPUTS / sizeof VALID_INPUTS[0];

void turnOnPin(String &);
bool validateInpupt(String &user_input);
void turnOffPins();
void promptColor();

void setup()
{
  // put your setup code here, to run once:
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.begin(9600);
  while (!Serial)
  {
  };
}

void loop()
{
  promptColor();

  while (!Serial.available())
  {
  }; // wait for input

  String user_input = Serial.readString();
  user_input.toLowerCase();

  bool input_is_valid = validateInpupt(user_input);
  if (input_is_valid)
  {
    turnOffPins();
    turnOnPin(user_input);
  }
  else
  {
    Serial.println("Wrong input boy! Try again...");
  }
}

void promptColor()
{
  Serial.print("Choose a color between ");
  Serial.print(VALID_INPUTS[0]);
  for (size_t i = 1; i < VALID_INPUTS_SIZE; i++)
  {
    Serial.print(",");
    Serial.print(VALID_INPUTS[i]);
  }
  Serial.println(": ");
}

void turnOffPins()
{
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
}

bool validateInpupt(String &user_input)
{
  for (size_t i = 0; i < VALID_INPUTS_SIZE; i++)
  {
    if (VALID_INPUTS[i].equalsIgnoreCase(user_input))
      return true;
  }
  return false;
}

void turnOnPin(String &user_input)
{
  char fl = user_input.c_str()[0];
  switch (fl)
  {
  case 'r':
    digitalWrite(RED_PIN, HIGH);
    break;

  case 'g':
    digitalWrite(GREEN_PIN, HIGH);
    break;

  case 'b':
    digitalWrite(BLUE_PIN, HIGH);
    break;

  case 'a':
    analogWrite(GREEN_PIN, 255);
    analogWrite(BLUE_PIN, 80);
    break;

  case 'f':
    analogWrite(RED_PIN, 255);
    analogWrite(BLUE_PIN, 255);
    break;

  default:
    break;
  }
}
