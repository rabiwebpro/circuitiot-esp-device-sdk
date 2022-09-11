#include "CircuitIoTContactSensor.h"

CircuitIoTContactSensor::CircuitIoTContactSensor(WiFiClientSecure& wifiClient, char token[]) : CircuitIoTDevice(wifiClient, token) {};

bool CircuitIoTContactSensor::pushContactStatus(const char* value){
  String publishTopic = String(_publishRoot + "/" + PROP_CONTACT).c_str();
  return publish(publishTopic.c_str(), value);
}
