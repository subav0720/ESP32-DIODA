#include "Logger.h"

#include <Arduino.h>

void loggerBegin(unsigned long baud) {
  Serial.begin(baud);
}

void loggerLine(const char* text) {
  Serial.println(text);
}
