#ifndef _CIRCUITIOT__CONTACT_SENSOR_H_
#define _CIRCUITIOT__CONTACT_SENSOR_H_

#include "CircuitIoTDevice.h"


#define PROP_CONTACT "contact"

class CircuitIoTContactSensor: public CircuitIoTDevice {
  
  public:  
    CircuitIoTContactSensor(WiFiClientSecure& wifiClient, char token[]);
    bool pushContactStatus(const char* value);
};

#endif
