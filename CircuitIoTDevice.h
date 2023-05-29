#ifndef _CIRCUITIOT_DEVICE_H_
#define _CIRCUITIOT_DEVICE_H_

#ifdef ESP8266 
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
       #include <WiFi.h>
#endif
#include <WiFiClientSecure.h>

#include "CircuitIoTMqtt.h"
#include "CircuitIoTUdp.h"
#include "CircuitIoTDecoder.h"
#include "CircuitIoTConfig.h"

#include <ArduinoJson.h>
#include <EEPROM.h>

#if defined(ESP8266) || defined(ESP32)
#include <functional>
#define DEVICE_CALLBACK_SIGNATURE std::function<void(const char*, const char*, const char*)> deviceCallback
#else
#define DEVICE_CALLBACK_SIGNATURE void (*deviceCallback)(const char*, const char*, const char*)
#endif


class CircuitIoTDevice: public CircuitIoTMqtt, public CircuitIoTUdp {
  public:
    CircuitIoTDevice(WiFiClientSecure& wifiClient, char token[]);
    void setDeviceCallback(DEVICE_CALLBACK_SIGNATURE);
    void setup();
    void run();
    String _deviceId;
    String _deviceSecret;
    String _deviceCategory;
    char* _token;
    String _devices[16]; 
    String _subscriptionRoot;
    String _publishRoot;
    int _deviceSize;  
  private:
    void parseToken();
    bool subscriptionApplicable();
    DEVICE_CALLBACK_SIGNATURE;    
  protected:
    virtual void onCallback(char* topic, byte* payload, unsigned int length);
        
};

#endif
