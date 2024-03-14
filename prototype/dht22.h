// need to download Adafruit DHT library first
#include "DHT.h"

#define DHTTYPE DHT22

// create an instance of your sensor outside the setup and loop functions of your program: DHT dht(DHTPIN, DHT22);
// also define a dht pin to use: e.g #define DHTPIN 27

DHT dht(27, DHT22);

// init sensor
void beginDHT() {
  dht.begin();
  return;
}

// read humidity
float readHumidity() {
  float h = dht.readHumidity();
  return h;
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
