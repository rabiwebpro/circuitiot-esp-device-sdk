#include <Arduino.h>
#ifdef ESP8266 
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
       #include <WiFi.h>
#endif

#include <CircuitIoTRGBStrip.h>

// Red, green, and blue pins for PWM control
const int redPin = 13;
const int greenPin = 12;
const int bluePin = 14;

//WiFi Status LED
#define wifiLed    D4   //D4

//SSID and Password
const char* ssid = "";
const char* password = "";

//Device token
char token[] = "";

WiFiClientSecure espClient;
CircuitIoTRGBStrip rgbStrip(espClient, token);

void setupWifi() {
 delay(10);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }

randomSeed(micros());
 
 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}

void setupRgbStrip() {  
 delay(10);
 rgbStrip.setup(redPin, greenPin, bluePin);
}


void setup() {
  Serial.begin(9600);
  pinMode(wifiLed, OUTPUT);
  
  //During Starting WiFi LED should TURN OFF
  digitalWrite(wifiLed, HIGH); 
  
  Serial.println("Connecting..."); 
  setupWifi();
  setupRgbStrip();
}

void loop() {
  if (!rgbStrip.connected()) {
    digitalWrite(wifiLed, HIGH);
  }
  else{
    digitalWrite(wifiLed, LOW);
  }
  rgbStrip.run();
}
