#ifndef _CIRCUITIOT_RGBWSTRIP_H_
#define _CIRCUITIOT_RGBWSTRIP_H_

#ifdef ESP8266 
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
       #include <WiFi.h>
#endif
#include <WiFiClientSecure.h>

#include "CircuitIoTDevice.h"
#include <EEPROM.h>


#define PROP_RGBW "rgbw"
#define PROP_POWER "power"
#define PROP_WHITE "white"

struct RGBW {
    int r,g,b,w;
};

class CircuitIoTRGBWStrip: public CircuitIoTDevice {
  
  public:  
    CircuitIoTRGBWStrip(WiFiClientSecure& wifiClient, char token[]);
    void setup(unsigned int redPin, unsigned int greenPin, unsigned int bluePin, unsigned int whitePin);
    void run();
    bool pushPowerStateChange(const char* value);
    bool pushColorChange(int red, int green, int blue, int white);
  private:
    int _redPin;
    int _greenPin;
    int _bluePin;
    int _whitePin;
    void parseRgbw(const char* value);
    void parseWhite(const char* whiteValue);
    void changeColor(int red, int green, int blue, int white);
    void handlePower(const char* value);
    struct RGBW _rgbw;
    void pwmWrite(int red, int green, int blue, int white);
  protected:
    virtual void onDeviceStateChange(const char* deviceId, const char* property, const char* value);
};

#endif
