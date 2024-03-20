// import the necessary libraries
#include <Preferences.h>
#include "TDS.h"
#include "dht22.h"
#include "pH.h"

#define pH_pin 25

Preferences preferences;
float EC = 0;

#define SAMPLING_PERIOD    500U  // ms
double last_measurement_ts = millis();

void setup() {
  // setup sensors
  setupTDS(26);
  beginDHT();
  Serial.setTimeout(10);
  // config values in nvs and calibrate sensors
  Serial.begin(115200);
  Serial.println();

  preferences.begin("references", true);  // we'll just store the ref values

  // to reset simulation
  //preferences.clear();

  int w = preferences.getInt("tds_water", 70);
  int m = preferences.getInt("tds_milk", 500);
  int e = preferences.getInt("tds_coolant", 800);

  int flag = preferences.getBool("iscalibrated", false);

  if (w >= 50 && w <= 300) {
    Serial.printf("water tds: %d", w);       //check if water tds within range
    if (m >= 100 & m <= 500) { 
      Serial.printf("milk tds: %d", m);    //if so check if milk tds within range
      if (e >= 300 && e <= 320) {  //if so check if coolang tds within range
        Serial.printf("coolant tds: %d", e);
      } else {
        Serial.println("coolant tds out of range. calibrating...");    
        e = 312;
        preferences.putInt("tds_coolant", e); // set to a value within range
      }
    } else {
      // set to a value within range
      Serial.println("milk tds out of range. calibrating...");
      m = 200;
      preferences.putInt("tds_milk", m);
    }
  } else {
    // set to a value within range
    Serial.println("water tds out of range. calibrating...");
    w = 100;
    preferences.putInt("tds_water", w);
  }
  flag = true;  //set to true if all values are within range
  preferences.putBool("iscalibrated", flag);
  Serial.println("the sensor is calibrated");

  delay(5000);
  ESP.restart();
}

void loop() {
  // read sensor data

  double now_ts = millis();

  if (now_ts - last_measurement_ts >= SAMPLING_PERIOD){

    last_measurement_ts = now_ts;
    
    // 1. read TDS and convert to EC
    readanalogvalues(analogBuffer);
    coeff = TDSCoeff(temperature);
    voltage = getFinalVoltage(voltage, coeff);
    tdsValue = getTDSValue(voltage);
    EC = tdsValue * 0.002; // EC = ppm * 2 / 1000. source: https://generalhydroponics.com/faqs/how-do-i-convert-between-tds-and-ec-readings/
    // 2. read pH
    readsample(26, buf);
    pHVolt = analogtovolt(avgValue);
    pHValue = getpH(pHVolt);
    // 3. read temperature and humidity
    H = readHumidity();
    T = readTemperature('c');
    exceptionDHT();
  // adjust data according to fuzzy logic
  }
}
