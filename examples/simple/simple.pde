#include <Arduino_NET.h>

int ledPin = 13;

Arduino_NET arduino;

void setup()
{
  arduino.begin();
  arduino.attach(messageComplete);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void messageComplete(byte command, int argc, char** argv)
{
  digitalWrite(ledPin, HIGH);
}

void loop()
{
  while(arduino.available() > 0) arduino.process(); 
}