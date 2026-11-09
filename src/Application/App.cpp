#include "App.h"

#include <Arduino.h>
#include <stdio.h>
#include "../Drivers/GpioDriver.h"
#include "../Middleware/Logger.h"

namespace {
constexpr uint8_t OUTPUT_PIN = 2;
constexpr unsigned long LOGGER_BAUD = 115200;
constexpr uint32_t GPIO_INTERVAL_MS = 1000;
constexpr uint32_t UPTIME_INTERVAL_MS = 5000;

bool outputHigh = false;
uint32_t lastGpioMs = 0;
uint32_t lastUptimeMs = 0;
}

void appInit() {
  // false znamena LOW. Vestavena LED na tomto pinu neni zarucena.
  outputHigh = false;
  gpioInitOutput(OUTPUT_PIN, outputHigh);

  loggerBegin(LOGGER_BAUD);
  loggerLine("ESP32 START");

  const uint32_t now = millis();
  lastGpioMs = now;
  lastUptimeMs = now;
}

void appProcess() {
  const uint32_t now = millis();

  // Odecitani unsigned casu funguje i pres preteceni millis().
  if ((uint32_t)(now - lastGpioMs) >= GPIO_INTERVAL_MS) {
    outputHigh = !outputHigh;
    gpioWrite(OUTPUT_PIN, outputHigh);
    lastGpioMs = now;
  }

  if ((uint32_t)(now - lastUptimeMs) >= UPTIME_INTERVAL_MS) {
    char message[32];
    snprintf(message, sizeof(message), "Uptime: %lu s",
             static_cast<unsigned long>(now / 1000U));
    loggerLine(message);
    lastUptimeMs = now;
  }
}
