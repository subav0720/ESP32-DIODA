#include "GpioDriver.h"

#include <Arduino.h>

void gpioInitOutput(uint8_t pin, bool initialHigh) {
  // Pripravime hodnotu pred zapnutim vystupu.
  digitalWrite(pin, initialHigh ? HIGH : LOW);
  pinMode(pin, OUTPUT);
}

void gpioWrite(uint8_t pin, bool high) {
  digitalWrite(pin, high ? HIGH : LOW);
}
