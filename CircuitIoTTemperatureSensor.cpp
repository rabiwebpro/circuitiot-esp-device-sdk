#include "CircuitIoTTemperatureSensor.h"

CircuitIoTTemperatureSensor::CircuitIoTTemperatureSensor(WiFiClientSecure& wifiClient, char token[]) : CircuitIoTDevice(wifiClient, token) {};

bool CircuitIoTTemperatureSensor::pushTemperature(const char* value){
  String publishTopic = String(_publishRoot + "/" + PROP_TEMPERATURE).c_str();
  return publish(publishTopic.c_str(), value);
}

bool CircuitIoTTemperatureSensor::pushHumidity(const char* value){
  String publishTopic = String(_publishRoot + "/" + PROP_HUMIDITY).c_str();
  return publish(publishTopic.c_str(), value);
}
