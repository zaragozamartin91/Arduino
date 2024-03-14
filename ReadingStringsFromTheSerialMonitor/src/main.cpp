#include <Arduino.h>

const int RED_PIN = PIN3;
const int BLUE_PIN = PIN7;
const int YELLOW_PIN = 12;

String AVAILABLE_INPUTS[] = {"RED", "GREEN", "YELLOW"};
// how to find lenght of array: https://stackoverflow.com/questions/4108313/how-do-i-find-the-length-of-an-array
const size_t AVAILABLE_INPUTS_SIZE = sizeof AVAILABLE_INPUTS / sizeof AVAILABLE_INPUTS[0];
const int INPUT_PIN_MAP[] = {RED_PIN, BLUE_PIN, YELLOW_PIN};

class AvailableInput
{
private:
  String *availableInputs;
  int availableInputsSize;
  int *inputPinMap;

public:
  AvailableInput()
  {
    availableInputs = new String[3]{"RED", "GREEN", "YELLOW"};
    availableInputsSize = 3;
    inputPinMap = new int[3]{RED_PIN, BLUE_PIN, YELLOW_PIN};
  }

  ~AvailableInput()
  {
    delete[] availableInputs;
    delete[] inputPinMap;
  }
};

AvailableInput AVAILABLE_INPUT;

char STR_BUFFER[128];

int parsePin(String &);
void turnOffAllPins();

void setup()
{
  // put your setup code here, to run once:
  pinMode(RED_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);

  Serial.begin(9600);
  while (!Serial)
  {
  };
}

void loop()
{
  Serial.println("Choose a pin ; RED, GREEN or YELLOW: ");

  // waiting for info on serial monito
  while (!Serial.available())
  {
  }

  String user_input = Serial.readString();

  sprintf(STR_BUFFER, "Your color is %s right?", user_input.c_str());
  Serial.println(STR_BUFFER);

  int selected_pin = parsePin(user_input);
  if (selected_pin == 0)
  {
    Serial.println("Wrong pin pal, try again!");
  }
  else
  {
    turnOffAllPins();
    digitalWrite(selected_pin, HIGH);
  }
}

int parsePin(String &user_input)
{
  for (unsigned int i = 0; i < AVAILABLE_INPUTS_SIZE; i++)
  {
    String *ai = &AVAILABLE_INPUTS[i];
    if (ai->equalsIgnoreCase(user_input))
    {
      return INPUT_PIN_MAP[i];
    }
  }
  return 0;
}

void turnOffAllPins()
{
  for (size_t i = 0; i < AVAILABLE_INPUTS_SIZE; i++)
  {
    digitalWrite(INPUT_PIN_MAP[i], LOW);
  }
}