#ifndef __DHT_H__
#define __DHT_H__

#include "config.h"

// need to download Adafruit DHT library first
#include "DHT.h"
#define DHTTYPE DHT22

DHT dht(DHT_PIN, DHT22);

// init sensor
void beginDHT() {
  dht.begin();
  return;
}

// read humidity
float readHumidity() {
  #ifdef SIM
    return random(3);
  #else
    float h = dht.readHumidity();
    return h;
  #endif
}

// read temperature
float readTemperature(char x) {
  // temperature read in °C
  float temp = dht.readTemperature();
  if (toupper(x) == 'F') {
    temp = temp * 9 / 5 + 32;  // Convert to Fahrenheit
  }
  else if (toupper(x) != 'C') {
    Serial.println("Undefined unit...");
    return -1;
  }
   return temp;
}

// error handling
void exceptionDHT() {
  if (isnan(dht.readTemperature()) || isnan(dht.readTemperature(true)) || isnan(dht.readHumidity())) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
}
#endif // __DHT_H__