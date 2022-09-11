#include "CircuitIoTRGBStrip.h"

CircuitIoTRGBStrip::CircuitIoTRGBStrip(WiFiClientSecure& wifiClient, char token[]) : CircuitIoTDevice(wifiClient, token) {
  CircuitIoTDevice::setDeviceCallback([this] (const char* deviceId, const char* property, const char* value) { this->onDeviceStateChange(deviceId, property, value); });
 };

void CircuitIoTRGBStrip::setup(unsigned int redPin, unsigned int greenPin, unsigned int bluePin){
  this->_redPin = redPin;
  this->_greenPin = greenPin;
  this->_bluePin = bluePin;

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
        
        // attach the channel to the GPIO to be controlled
        ledcAttachPin(this->_redPin, 0);
        ledcAttachPin(this->_greenPin, 1);
        ledcAttachPin(this->_bluePin, 2);
    #endif
  
  
  EEPROM.begin(512);
  
  EEPROM.get(0, this->_rgb);
  if(this->_rgb.r == NAN){
    this->_rgb = (RGB){255,0,0};
    EEPROM.put(0, this->_rgb);
    delay(100);
  }else{
    pwmWrite(this->_rgb.r, this->_rgb.g, this->_rgb.b);
  }  
  
  CircuitIoTDevice::setup();
}


void CircuitIoTRGBStrip::run(){
  
  CircuitIoTDevice::run();
}

void CircuitIoTRGBStrip::onDeviceStateChange(const char* deviceId, const char* property, const char* value) {  
  if(strcmp(property, PROP_RGB) == 0) parseRgb(value);
  if(strcmp(property, PROP_POWER) == 0) handlePower(value);
}

void CircuitIoTRGBStrip::parseRgb(const char* rgbString){  
    StaticJsonDocument<512> doc;
    deserializeJson(doc, rgbString);    
    int r = doc["r"];
    int g = doc["g"];
    int b = doc["b"];
    changeColor(r,g,b);
}

void CircuitIoTRGBStrip::changeColor(int red, int green, int blue){
  pwmWrite(red, green, blue);
  
  this->_rgb = (RGB){red,green,blue};
  EEPROM.put(0, this->_rgb);
  delay(100);
}

void CircuitIoTRGBStrip::handlePower(const char* value){
  if (strcmp(value, "1") == 0){
    pwmWrite(this->_rgb.r, this->_rgb.g, this->_rgb.b);    
    pushColorChange(this->_rgb.r, this->_rgb.g, this->_rgb.b);
  }else{
    pwmWrite(0, 0, 0);
  }
}

bool CircuitIoTRGBStrip::pushPowerStateChange(const char* value){
  String publishTopic = String(_publishRoot + "/"+PROP_POWER).c_str();
  Serial.printf("Publishing to [%s] ", publishTopic.c_str());
  return sendMessage(publishTopic.c_str(), value);
}

bool CircuitIoTRGBStrip::pushColorChange(int red, int green, int blue){
  DynamicJsonDocument doc(1024);

  doc["r"] = red;
  doc["g"] = green;
  doc["b"] = blue;
  
  String value;
  serializeJson(doc, value);
  String publishTopic = String(_publishRoot + "/"+PROP_RGB).c_str();
  Serial.printf("Publishing to [%s] ", publishTopic.c_str());
  return sendMessage(publishTopic.c_str(), value.c_str());
}

void CircuitIoTRGBStrip::pwmWrite(int red, int green, int blue){
  #if defined(ESP8266) 
        analogWrite(this->_redPin, red);
        analogWrite(this->_greenPin, green);
        analogWrite(this->_bluePin, blue);
    #endif 
    #if defined(ESP32)
      ledcWrite(0, red);
      ledcWrite(1, green);
      ledcWrite(2, blue);
    #endif
}
