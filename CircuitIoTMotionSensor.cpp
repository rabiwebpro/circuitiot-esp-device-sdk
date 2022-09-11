#include "CircuitIoTMotionSensor.h"

CircuitIoTMotionSensor::CircuitIoTMotionSensor(WiFiClientSecure& wifiClient, char token[]) : CircuitIoTDevice(wifiClient, token) {};

bool CircuitIoTMotionSensor::pushMotionDetected(const char* value){
  String publishTopic = String(_publishRoot + "/" + PROP_MOTION).c_str();
  return publish(publishTopic.c_str(), value);
}
