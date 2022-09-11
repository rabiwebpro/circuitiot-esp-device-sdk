#ifndef _CIRCUITIOT__TEMPERATURE_SENSOR_H_
#define _CIRCUITIOT__TEMPERATURE_SENSOR_H_

#include "CircuitIoTDevice.h"


#define PROP_TEMPERATURE "temperature"
#define PROP_HUMIDITY "humidity"

class CircuitIoTTemperatureSensor: public CircuitIoTDevice {
  
  public:  
    CircuitIoTTemperatureSensor(WiFiClientSecure& wifiClient, char token[]);
    bool pushTemperature(const char* value);
    bool pushHumidity(const char* value);
};

#endif
