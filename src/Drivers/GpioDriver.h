#pragma once

#include <stdint.h>

// Nastavi digitalni vystup a jeho pocatecni stav (true = HIGH).
void gpioInitOutput(uint8_t pin, bool initialHigh);
void gpioWrite(uint8_t pin, bool high);
