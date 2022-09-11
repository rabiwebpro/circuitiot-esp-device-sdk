#include "CircuitIoTMultiSwitch.h"

CircuitIoTMultiSwitch::CircuitIoTMultiSwitch(WiFiClientSecure& wifiClient, char token[]) : CircuitIoTDevice(wifiClient, token) {
    CircuitIoTDevice::setDeviceCallback([this] (const char* deviceId, const char* property, const char* value) { this->onDeviceStateChange(deviceId, property, value); });
 };

void CircuitIoTMultiSwitch::onDeviceStateChange(const char* deviceId, const char* property, const char* value) {

  int deviceIndex = 0;
  for (int i =1; i< _deviceSize; i++) {
    if (strcmp(deviceId, _devices[i].c_str()) == 0) {
      deviceIndex = i;
      break;
    }
  }
  
  if (powerStateCallback != NULL && (String(property) == PROP_POWER)) powerStateCallback(deviceIndex, value);
}


void CircuitIoTMultiSwitch::onPowerStateChange(POWER_STATE_CALLBACK_SIGNATURE)
{
    this->powerStateCallback = powerStateCallback;
}

bool CircuitIoTMultiSwitch::pushPowerStateChange(int deviceIndex, const char* value){
  String subDevice = _devices[deviceIndex];
  String publishTopic = String(_publishRoot + "/" + subDevice + "/" + PROP_POWER).c_str();
  return publish(publishTopic.c_str(), value);
}
