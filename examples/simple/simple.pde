#include <Arduino_NET.h>

#define Command1 0x01
#define Command2 0x02
#define Command3 0x03

int ledPin = 13;
char buf[5];

void setup()
{
  Arduino.begin(19200);
  Arduino.attach(messageComplete);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void messageComplete(byte command, int argc, char** argv)
{
  switch(command)
  {
    case Command1:
      digitalWrite(ledPin, HIGH);
      Arduino.sendCommand(Command1);
      break;
    case Command2:
      digitalWrite(ledPin, LOW);
      Arduino.sendCommand(Command2);
      break;
    case Command3:
      delay(atoi(argv[0]));
      digitalWrite(ledPin, HIGH);
      Arduino.sendCommand(Command3, argv[0]);
  }
}

void loop()
{
  while(Arduino.available() > 0) Arduino.process(); 
}