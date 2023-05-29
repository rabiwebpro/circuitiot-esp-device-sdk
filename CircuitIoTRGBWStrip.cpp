#include "CircuitIoTRGBWStrip.h"

CircuitIoTRGBWStrip::CircuitIoTRGBWStrip(WiFiClientSecure& wifiClient, char token[]) : CircuitIoTDevice(wifiClient, token) {
  CircuitIoTDevice::setDeviceCallback([this] (const char* deviceId, const char* property, const char* value) { this->onDeviceStateChange(deviceId, property, value); });
 };

void CircuitIoTRGBWStrip::setup(unsigned int redPin, unsigned int greenPin, unsigned int bluePin, unsigned int whitePin){
  this->_redPin = redPin;
  this->_greenPin = greenPin;
  this->_bluePin = bluePin;
  this->_whitePin = whitePin;

    #if defined(ESP8266) 
        const int resolution = 256;
        analogWriteRange(resolution);
    #endif 
    #if defined(ESP32)  
         const int freq = 5000;
         const int resolution = 8;
         // configure LED PWM functionalitites
        ledcSetup(0, freq, resolution);
        ledcSetup(1, freq, resolution);
        ledcSetup(2, freq, resolution);
        ledcSetup(3, freq, resolution);
        
        // attach the channel to the GPIO to be controlled
        ledcAttachPin(this->_redPin, 0);
        ledcAttachPin(this->_greenPin, 1);
        ledcAttachPin(this->_bluePin, 2);
        ledcAttachPin(this->_whitePin, 3);
    #endif
  
  
  EEPROM.begin(512);
  
  EEPROM.get(0, this->_rgbw);
  if(this->_rgbw.r == NAN){
    this->_rgbw = (RGBW){255,0,0,0};
    EEPROM.put(0, this->_rgbw);
    delay(100);
  }else{
    pwmWrite(this->_rgbw.r, this->_rgbw.g, this->_rgbw.b, this->_rgbw.w);
  }  
  
  CircuitIoTDevice::setup();
}


void CircuitIoTRGBWStrip::run(){
  
  CircuitIoTDevice::run();
}

void CircuitIoTRGBWStrip::onDeviceStateChange(const char* deviceId, const char* property, const char* value) {  
  if(strcmp(property, PROP_RGBW) == 0) parseRgbw(value);
  if(strcmp(property, PROP_POWER) == 0) handlePower(value);
}

void CircuitIoTRGBWStrip::parseWhite(const char* whiteValue){
  int white = atoi((char *)whiteValue);
  Serial.print("white value::::");
  Serial.print(white);
  pwmWrite(0,0,0,white);
}

void CircuitIoTRGBWStrip::parseRgbw(const char* rgbString){  
    StaticJsonDocument<512> doc;
    deserializeJson(doc, rgbString);    
    int r = doc["r"];
    int g = doc["g"];
    int b = doc["b"];
    int w = doc["w"];
    changeColor(r,g,b,w);
}

void CircuitIoTRGBWStrip::changeColor(int red, int green, int blue, int white){
  pwmWrite(red, green, blue, white);
  
  this->_rgbw = (RGBW){red,green,blue,white};
  EEPROM.put(0, this->_rgbw);
  delay(100);
}

void CircuitIoTRGBWStrip::handlePower(const char* value){
  if (strcmp(value, "1") == 0){
    pwmWrite(this->_rgbw.r, this->_rgbw.g, this->_rgbw.b, this->_rgbw.w);    
    pushColorChange(this->_rgbw.r, this->_rgbw.g, this->_rgbw.b, this->_rgbw.w);
  }else{
    pwmWrite(0, 0, 0, 0);
  }
}

bool CircuitIoTRGBWStrip::pushPowerStateChange(const char* value){
  String publishTopic = String(_publishRoot + "/"+PROP_POWER).c_str();
  Serial.printf("Publishing to [%s] ", publishTopic.c_str());
  return sendMessage(publishTopic.c_str(), value);
}

bool CircuitIoTRGBWStrip::pushColorChange(int red, int green, int blue, int white){
  DynamicJsonDocument doc(1024);

  doc["r"] = red;
  doc["g"] = green;
  doc["b"] = blue;
  doc["w"] = white;
  
  String value;
  serializeJson(doc, value);
  String publishTopic = String(_publishRoot + "/"+PROP_RGBW).c_str();
  Serial.printf("Publishing to [%s] ", publishTopic.c_str());
  return sendMessage(publishTopic.c_str(), value.c_str());
}

void CircuitIoTRGBWStrip::pwmWrite(int red, int green, int blue, int white){
  #if defined(ESP8266) 
        analogWrite(this->_redPin, red);
        analogWrite(this->_greenPin, green);
        analogWrite(this->_bluePin, blue);
        analogWrite(this->_whitePin, white);
    #endif 
    #if defined(ESP32)
      ledcWrite(0, red);
      ledcWrite(1, green);
      ledcWrite(2, blue);
      ledcWrite(3, white);
    #endif
}
