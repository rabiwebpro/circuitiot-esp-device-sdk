#ifndef _CIRCUITIOT__CONTACT_METER_H_
#define _CIRCUITIOT__CONTACT_METER_H_

#ifdef ESP8266 
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
       #include <WiFi.h>
#endif
#include <WiFiClientSecure.h>

#include "CircuitIoTDevice.h"
#include <EEPROM.h>


#define PROP_RGB "rgb"
#define PROP_POWER "power"

struct RGB {
    int r,g,b;
};

class CircuitIoTRGBStrip: public CircuitIoTDevice {
  
  public:  
    CircuitIoTRGBStrip(WiFiClientSecure& wifiClient, char token[]);
    void setup(unsigned int redPin, unsigned int greenPin, unsigned int bluePin);
    void run();
    bool pushPowerStateChange(const char* value);
    bool pushColorChange(int red, int green, int blue);
  private:
    int _redPin;
    int _greenPin;
    int _bluePin;
    void parseRgb(const char* value);
    void changeColor(int red, int green, int blue);
    void handlePower(const char* value);
    struct RGB _rgb;
    void pwmWrite(int red, int green, int blue);
  protected:
    virtual void onDeviceStateChange(const char* deviceId, const char* property, const char* value);
};

#endif
