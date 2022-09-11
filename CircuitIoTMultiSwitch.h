#ifndef _CIRCUITIOT__MULTI_SWITCH_H_
#define _CIRCUITIOT__MULTI_SWITCH_H_

#include "CircuitIoTDevice.h"

#if defined(ESP8266) || defined(ESP32)
#include <functional>
#define POWER_STATE_CALLBACK_SIGNATURE std::function<void(int, const char*)> powerStateCallback
#else
#define POWER_STATE_CALLBACK_SIGNATURE void (*powerStateCallback)(int, const char*)
#endif
#define PROP_POWER "power"

class CircuitIoTMultiSwitch: public CircuitIoTDevice {
  
  public:  
    CircuitIoTMultiSwitch(WiFiClientSecure& wifiClient, char token[]);
    void onPowerStateChange(POWER_STATE_CALLBACK_SIGNATURE);
    bool pushPowerStateChange(int deviceIndex, const char* value);
  private:
    POWER_STATE_CALLBACK_SIGNATURE;
  protected:
    virtual void onDeviceStateChange(const char* deviceId, const char* property, const char* value);
};

#endif
