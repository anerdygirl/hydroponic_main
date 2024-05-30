/* defines pinouts and global variables */
#ifndef __CONFIG_H__
#define __CONFIG_H__

#define SIM     // for the sake of demo cuz my sensors are crap

// sensors pinout
#define PH_PIN             25
#define TDS_PIN            4
#define DHT_PIN            26

// relay module pinout
#define WATER_PUMP_PIN     27
#define V1_PIN             16    // mix A valve
#define V2_PIN             17    // mix B valve
#define V3_PIN             18    // freshwater valve
#define V4_PIN             19    // tank drain valve
//#define LEDLIGHTS_PIN      16

// Define timing intervals
#ifdef SIM
  #define intervalSensor                  3 * 60 * 1000UL
  #define interval24hr                    10 * 60 * 1000UL
#else
  #define interval24hr                    24 * 60 * 60 * 1000UL
  #define intervalSensor                  60 * 60 * 1000UL
#endif
extern unsigned long lastSensorMillis = 0;
unsigned long last24hrMillis = 0;

// valves
#ifdef SIM
#define valveOperationTime              3000
#else
#define valveOperationTime              42860 // in ms
#endif

#endif