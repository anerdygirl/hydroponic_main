// import the necessary libraries
#include "config.h"
#include "TDS.h"
#include "dht22.h"
#include "pH.h"
#include "pump.h"
#include "mqtt.h"

float tempValue, h_Value;
bool valveOperated = false;


// to maintain mqtt connection
unsigned long lastMQTTCheck = 0;
const unsigned long mqttCheckInterval = 10000;  // 10 seconds

void setup() {
  // Serial.. since i don't have lcd or sth to display data on
  Serial.begin(115200);
  // Serial.setTimeout(SER_TIMEOUT);

  // setup sensors
  setupTDS(TDS_PIN);
  beginDHT();

  // init relay module
  pinMode(V1_PIN, OUTPUT);
  pinMode(V2_PIN, OUTPUT);
  pinMode(V3_PIN, OUTPUT);
  pinMode(V4_PIN, OUTPUT);

  // init water pump
  initPump(V1_PIN);

  // init mqtt connection
  wifiInit();
  connectToMQTT();

  // init valves - off
  digitalWrite(V1_PIN, HIGH);
  digitalWrite(V2_PIN, HIGH);
  digitalWrite(V3_PIN, HIGH);
}

void loop() {

  // make sure connection to adafruit io isn't lost
  if (!mqtt.connected()) {
    connectToMQTT();  // Attempt to reconnect if not connected
  }
  unsigned long currentMillis = millis();
  if (currentMillis - lastMQTTCheck >= mqttCheckInterval) {
    lastMQTTCheck = currentMillis;
    mqtt.processPackets(1000);  // Process packets for 1 seconds
  }

  // operate every 24hrs -- or 5min for demo purposes
  if (currentMillis - last24hrMillis >= interval24hr) {
    last24hrMillis = currentMillis;
    valveOperated = false;  // Reset valve operation flag
  }

  // Sensor measurements every hour
  if (currentMillis - lastSensorMillis >= intervalSensor) {
    lastSensorMillis = currentMillis;

    // 1. read TDS
    tdsValue = getTDSValue();
    Serial.println(tdsValue);

    // 2. read pH
    pHValue = getpH();
    Serial.println(pHValue);

    // 3. read temperature and humidity
    h_Value = readHumidity();
    tempValue = readTemperature('c');
    exceptionDHT();

    // send sensor info to adafruit io
    sendDataToAdafruitIO(pHValue, tdsValue, tempValue, h_Value);

    // if out of range, operate valves for about 42s after the first sensor reading of the 24-hour cycle
    if (!valveOperated) {
      if (pHValue < 5.5) {
        //operate v1 and v2
        digitalWrite(V1_PIN, LOW);
        digitalWrite(V2_PIN, LOW);
        delay(valveOperationTime);
        digitalWrite(V1_PIN, HIGH);
        digitalWrite(V2_PIN, HIGH);
      }
      if (tdsValue < 250) {
        // operate v3
        digitalWrite(V3_PIN, LOW);
        delay(valveOperationTime);
        digitalWrite(V3_PIN, HIGH);
      }
      valveOperated = true;
    }

    // Manage the water pump operation --- after i fix the tank leaking
    // waterPump(V1_PIN, currentMillis);

    /* Manage the LED light operation
     manageLEDLights(currentMillis); */
  }

}