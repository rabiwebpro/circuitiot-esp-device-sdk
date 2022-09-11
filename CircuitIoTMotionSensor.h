#ifndef _CIRCUITIOT__MOTION_SENSOR_H_
#define _CIRCUITIOT__MOTION_SENSOR_H_

#include "CircuitIoTDevice.h"


#define PROP_MOTION "motion"

class CircuitIoTMotionSensor: public CircuitIoTDevice {
  
  public:  
    CircuitIoTMotionSensor(WiFiClientSecure& wifiClient, char token[]);
    bool pushMotionDetected(const char* value);
};

#endif
