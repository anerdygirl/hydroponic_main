#ifndef __PH_H__
#define __PH_H__
#include "config.h"
void readsample(int AnalogPin, int (&buf)[10]);  // Function declaration with reference parameter

// variables
int sensorValue = 0;  // init reading
unsigned long int avgValue = 0;
int buf[10], temp;
float pHVolt = 0;
float pHValue = 0;

// get 10 readings
void readsample(int AnalogPin, int (&buf)[10]) {
  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(AnalogPin);
    delay(10);
  }
}

// sort in ascending order
void sortAscend(int (&buf)[10]){
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
}

// Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
float analogtovolt(unsigned long int avg) {
  float pHVolt = (float)avg * 5.0 / 1024 / 6;
  return pHVolt;
}

// find pH according to the sensor equation: y= -5.70 * x + 21.34
float getpH() {
  #ifdef SIM
    float x = random(1401) / 100.0;;
    return x; // from 0 to 14
  #else
    readsample(PH_PIN, buf);
    pHVolt = analogtovolt(avgValue);
    float ph = -5.70 * pHVolt + 21.34;
    return ph;
  #endif
}

#endif // __PH_H__