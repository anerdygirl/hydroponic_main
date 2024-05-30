#ifndef __PUMP_H__
#define __PUMP_H__

#include "config.h"

unsigned long lastPumpSubMillis = 0;
bool pumpSubOn = true;
bool subCycle = false;
const unsigned long intervalPumpOn = 60 * 60 * 1000UL;  // 1 hour
const unsigned long intervalPumpOff = 5 * 60 * 1000UL;  // 5 minutes


void initPump(int pump_pin){
  digitalWrite(pump_pin, HIGH);
}

void waterPump(int pump_pin, unsigned long currentMillis) {
  // Check if 24-hour cycle is in pump on period
  if (currentMillis - last24hrMillis < interval24hr / 2) {
    if (!subCycle) {
      subCycle = true;
      lastPumpSubMillis = currentMillis;
      pumpSubOn = true;
      digitalWrite(pump_pin, LOW);
    }
    // turn off pump if on
    if (pumpSubOn) {
      if (currentMillis - lastPumpSubMillis >= intervalPumpOn) {
        lastPumpSubMillis = currentMillis;
        pumpSubOn = false;
        digitalWrite(pump_pin, HIGH);
      }
    // turn on pump if off
    } else {
      if (currentMillis - lastPumpSubMillis >= intervalPumpOff) {
        lastPumpSubMillis = currentMillis;
        pumpSubOn = true;
        digitalWrite(pump_pin, LOW);
      }
    }
  } else {
    if (subCycle) {
      subCycle = false;
      digitalWrite(pump_pin, HIGH);
    }
  }
}

#endif