// import the necessary libraries
#include <Preferences.h>
#include "TDS.h"
#include "dht22.h"
#include "pH.h"
#include "fuzzy.h"
#include "config.h"

Preferences preferences;
float WL; // ultrasonic sensor or equivalent is needed for water level

double last_measurement_ts = millis();

void setup() {
  Serial.begin(115200);
  // setup sensors
  setupTDS(TDS_PIN);
  beginDHT();
  Serial.setTimeout(SER_TIMEOUT);

  // setup fuzzy system
  setup_fuzzy();

  // config values in nvs and calibrate sensors. to be implemented....
}

void loop() {

  // read sensors data
  double now_ts = millis();
  if (now_ts - last_measurement_ts >= SAMPLING_PERIOD){
    last_measurement_ts = now_ts;
    
    // 1. read EC
    tdsValue = getTDSValue();
    EC = getECValue(tdsValue);
    
    // 2. read pH
    pHValue = getpH();

    // 3. read temperature and humidity
    H = readHumidity();
    T = readTemperature('c');
    exceptionDHT();

  // adjust data according to fuzzy logic
  set_fuzzy_inputs(pHValue, EC, WL);
  fuzzify_sys();
  fuzzyoutputs outputs = defuzzify_sys();
  // next step??
  }
}
