# hydroponic_main

Main program for my automated hydroponics system on ESP32, structured in modules:

- **TDS.h, pH.h and DHT22.h** are the drivers for the sensors used. These can be found in their respective repos (with some changes for simulation/demo purposes).
- **config.h** contains the declarations of global variables and pinouts.
- **pump.h** to automate water pump scheduling.
- **prototype.ino** is the main program.

*Removed mqtt.h header for security reasons, but it's responsible for the MQTT connection to Adafruit IO.*
