// import the necessary libraries
#include <Preferences.h>
#include "TDS.h"
#include "dht22.h"
#include "pH.h"

#define pH_pin 25

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
  // adjust data according to fuzzy logic
}

/* fuzzy rules:
[WT: water tank; N: nutrients; D: drain]
if ph low and ec low and water low then WT on and N on and D off
if ph low and ec low and water within then WT on and N on and D off
if ph low and ec low na water full then WT on and N on and D on
if ph low and ec within and water low then WT on and N on and D off
if ph low and ec within and water within then WT on and N on and D off
if ph low and ec within and water full then WT on and N on and D on
if ph low and ec high and water low then WT on and N on and D off
if ph low and ec high and water within then WT on and N on and D off
if ph low and ec high and water low then WT on and N on and D on
if ph in range and ec low and water low then WT off and N on and D off
if ph in range and ec low and water within then WT off and N on and D off
if ph in range and ec low na water full then WT off and N on and D on
if ph in range and ec within and water low then WT off and N on and D off
if ph in range and ec within and water within then WT off and N on and D off
if ph in range and ec within and water full then WT off and N on and D on
if ph in range and ec high and water low then WT off and N on and D off
if ph in range and ec high and water within then WT off and N on and D off
if ph in range and ec high and water low then WT off and N on and D on
if ph above and ec low and water low then WT off and N on and D off
if ph above and ec low and water within then WT off and N on and D off
if ph above and ec low and water full then then WT off and N on and D on
if ph above and ec within and water low then WT off and N on and D off
if ph above and ec within and water within then WT off and N on and D off
if ph above and ec within and water full then WT off and N on and D on
if ph above and ec high and water low then WT off and N on and D off
if ph above and ec high and water within then WT off and N on and D off
if ph above and ec high and water low then WT off and N on and D on
*/