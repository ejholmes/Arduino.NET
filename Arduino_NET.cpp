/*
 *   This file is part of Arduino.NET
 *
 *   Arduino.NET is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Foobar is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

//ADDED FOR COMPATIBILITY WITH WIRING
extern "C" {
  #include <stdlib.h>
}

#include "WProgram.h"
#include "Arduino_NET.h"

#define ARG_SEPARATOR     0x20 // Space

Arduino_NET::Arduino_NET(void) {
  argCount = 0;
  bufferIndex = 0;
}

void Arduino_NET::attach(messageCompleteCallback func) {
  currentCallback = func;
}

void Arduino_NET::begin(int baud) {
  Serial.begin(baud);
}

int Arduino_NET::available() {
  return Serial.available();
}

void Arduino_NET::sendCommand(byte command, const char* argv) {
  Serial.print(command, BYTE);
  Serial.print(ARG_SEPARATOR, BYTE);
  if(argv != NULL)
    Serial.print(argv);
  Serial.print(ARG_SEPARATOR, BYTE);
  Serial.println(END_MESSAGE, BYTE);
}

void Arduino_NET::sendCommand(byte command)
{
  sendCommand(command, NULL);
}

char** Arduino_NET::splitArgs(char *list) {
  int i, count = 0;
  for(i = 0; list[i] != '\0'; i++) {
    if(list[i] == ARG_SEPARATOR)
      count++;
  }
  count--;
  argCount = count;
  
  char** split = (char**)calloc(BUFFERSIZE, sizeof(char*));
  const char separator[] = { (char)ARG_SEPARATOR };
  strtok(list, separator);
  
  for(i=0; i < count; i++) {
    split[i] = strtok(NULL, separator);
  }
  
  return split;
}

void Arduino_NET::process() {
  char serialChar = Serial.read();
  
  if(serialChar > 0) {
    if(serialChar == END_MESSAGE) {
      if(buffer[bufferIndex-1] != ESCAPE_CHARACTER) {
        char** args = splitArgs(buffer);
        (*currentCallback)((byte)buffer[0], argCount, args);
        bufferIndex = 0;
      }
      else {
        buffer[bufferIndex-1] = serialChar;
      }
    }
    else {
      buffer[bufferIndex] = serialChar;
      bufferIndex++;
    }
  }
}
