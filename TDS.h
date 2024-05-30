#ifndef __TDS_H__
#define __TDS_H__
#include "config.h"
#define VREF         3.3   // analog reference voltage(Volt) of the ADC. using the ESP32's built-in ADC
#define SCOUNT       30  

void readvalues(int (&analogBuffer)[SCOUNT]);  // Function declaration with reference parameter
float avgVoltage(int buffer[], int count, float vref);
float getMedian(int bArray[], int iFilterLen);

// initialize variables
int analogBuffer[SCOUNT];      // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];  // analogBuffer after applying median filtering
int analogBufferIndex = 0;
float voltage = 0;
float tdsValue;
float temperature = 16;  // current temperature for compensation (arbitrary value). you can use a temperature sensor and adjust as needed
float coeff;
float EC;
float conversion_coeff = 0.002;

void setupTDS(int pin) {
  pinMode(pin, INPUT);
}

static unsigned long analogSampleTimepoint = millis(); // start time for reading analog values
void readanalogvalues(int (&analogBuffer)[SCOUNT]) {
  // read a bunch of analog values and store them in the buffer every 40ms
  if (millis() - analogSampleTimepoint > 40U) {
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(TDS_PIN);  //read the analog value and store into the buffer
    analogBufferIndex++;
    if (analogBufferIndex == SCOUNT) {
      analogBufferIndex = 0;
    }
  }
  // get the latest readings and average voltage every 800ms
  static unsigned long printTimepoint = millis();
  if (millis() - printTimepoint > 800U) {
    printTimepoint = millis();
    for (int copyIndex = 0; copyIndex < SCOUNT; copyIndex++) {
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
      voltage = avgVoltage(analogBufferTemp, SCOUNT, VREF);
    }
  }
}

// takes temperature into account to compensate for the temperature variations
float getFinalVoltage(float avg, float coeff) {
  float voltage = avg / coeff;
  return voltage;
}

// calculates temperature coefficient
float TDSCoeff(float temperature) {
  float coeff = 1.0 + 0.02 * (temperature - 25.0);
  return coeff;
}

// calculates average voltage read from the sensor after applying median filtering and voltage conversion
float avgVoltage(int buffer[], int count, float vref) {
  float avg = getMedian(buffer, count) * vref / 1024.0;
  return avg;
}

// filters an array of readings to get a stable analog value
float getMedian(int bArray[], int iFilterLen) {
  // copy bArray into bTab
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  // sort bTab in ascending order
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  // apply median filtering according to the parity of array length
  if ((iFilterLen & 1) > 0) {
    bTemp = bTab[(iFilterLen - 1) / 2];
  } else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
  return bTemp;
}

// converts voltage to TDS value
float getTDSValue() {
  #ifdef SIM
  float tdsValue = random(0, 500001) / 1000.0;
  #else
  readanalogvalues(analogBuffer);
  coeff = TDSCoeff(temperature);
  voltage = getFinalVoltage(voltage, coeff);
  //gotta figure out the correct formula.. or simply paste it as is for now
  tdsValue = (133.42 * voltage * voltage * voltage - 255.86 * voltage * voltage + 857.39 * voltage) * 0.5;
  #endif
  return tdsValue;
}

float getECValue(float tds){
  #ifdef SIM
    float x = random(15) /10.0;
    return x; //from 0 to 1.4
  #else
    float ec = tds * conversion_coeff; // EC = ppm * 2 / 1000. source: https://generalhydroponics.com/faqs/how-do-i-convert-between-tds-and-ec-readings/
    return ec;
  #endif
}
#endif