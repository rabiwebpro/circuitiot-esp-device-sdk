#include "CircuitIoTSwitch.h"

CircuitIoTSwitch::CircuitIoTSwitch(WiFiClientSecure& wifiClient, char token[]) : CircuitIoTDevice(wifiClient, token) {
    //onPowerStateChange(NULL);  
    CircuitIoTDevice::setDeviceCallback([this] (const char* deviceId, const char* property, const char* value) { this->onDeviceStateChange(deviceId, property, value); });
 };

void CircuitIoTSwitch::onDeviceStateChange(const char* deviceId, const char* property, const char* value) {
 if (powerStateCallback != NULL && (String(property) == PROP_POWER)) powerStateCallback(value);
}

void CircuitIoTSwitch::onPowerStateChange(POWER_STATE_CALLBACK_SIGNATURE)
{
    this->powerStateCallback = powerStateCallback;
}

bool CircuitIoTSwitch::pushPowerStateChange(const char* value){
  String publishTopic = String(_publishRoot + "/power").c_str();
  Serial.printf("Publishing to [%s] ", publishTopic.c_str());
  return sendMessage(publishTopic.c_str(), value);
}
