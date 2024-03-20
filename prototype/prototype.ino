// import the necessary libraries
#include <Preferences.h>
#include "TDS.h"
#include "dht22.h"
#include "pH.h"
#include "fuzzy.h"

#define pH_pin 25

Preferences preferences;
float EC = 0;
float WL; // ultrasonic sensor or equivalent is needed for water level

#define SAMPLING_PERIOD    500U  // ms
double last_measurement_ts = millis();

void setup() {
  Serial.begin(115200);
  // setup sensors
  setupTDS(26);
  beginDHT();
  Serial.setTimeout(10);

  // setup fuzzy system
  setup_fuzzy();

  // config values in nvs and calibrate sensors. to be implemented....
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
  set_fuzzy_inputs(pHValue, EC, WL);
  fuzzify_sys();
  fuzzyoutputs outputs = defuzzify_sys();

  }
}
