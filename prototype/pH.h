#ifndef PH_H
#define PH_H
void readsample(int AnalogPin, int (&buf)[10]);  // Function declaration with reference parameter
#endif

// variables
int sensorValue = 0;  // init reading
unsigned long int avgValue = 0;
float b;
int buf[10], temp;

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
  float pHVol = (float)avg * 5.0 / 1024 / 6;
  return pHVol;
}

// find pH according to the sensor equation: y= -5.70 * x + 21.34
float getpH(float volt) {
  float ph = -5.70 * volt + 21.34;
  return ph;
}