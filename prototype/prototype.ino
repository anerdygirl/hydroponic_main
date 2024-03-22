// import the necessary libraries
#include <Preferences.h>
#include "TDS.h"
#include "dht22.h"
#include "pH.h"
#include "fuzzy.h"
#include "config.h"

Preferences preferences;
float WL; // ultrasonic sensor or equivalent is needed for water level

unsigned long last_measurement_ts = millis();

void setup() {
  Serial.begin(9600);
  // setup sensors
  setupTDS(TDS_PIN);
  beginDHT();
  Serial.setTimeout(SER_TIMEOUT);

  // setup leds to simulate fuzzy sys
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  // setup fuzzy system
  setup_fuzzy();

  // config values in nvs and calibrate sensors. to be implemented....
}

void loop() {

  // read sensors data
  unsigned long now_ts = millis();
  if (now_ts - last_measurement_ts >= SAMPLING_PERIOD){
    last_measurement_ts = now_ts;
    
    // 1. read EC
    tdsValue = getTDSValue();
    EC = getECValue(tdsValue);
    Serial.println(EC);
    
    // 2. read pH
    pHValue = getpH();
    Serial.println(pHValue);

    // 3. read temperature and humidity
    H = readHumidity();
    T = readTemperature('c');
    exceptionDHT();

    // 4. read water level
    #ifdef SIM
      WL = 60; //from 0 to 80
    #endif
    Serial.println(WL);


  // adjust data according to fuzzy logic
  set_fuzzy_inputs(pHValue, EC, WL);
  fuzzify_sys();
  fuzzyoutputs outputs = defuzzify_sys();
  // next step??
  //sim
  #ifdef SIM
    digitalWrite(RED_PIN, outputs.i_wt);
    digitalWrite(GREEN_PIN, outputs.i_nt);
    digitalWrite(BLUE_PIN, outputs.i_d);
  #endif
  }
}