#include <Arduino.h>
#ifdef ESP8266 
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
       #include <WiFi.h>
#endif
#include "DHT.h"

#include <CircuitIoTTemperatureSensor.h>

#define SensorPin D2  //D2

//WiFi Status LED
#define wifiLed    D4   //D4

//SSID and password
const char* ssid = "";
const char* password = "";

float temperature=0.0;
float humidity=0.0;

//Device token
char token[] = "";

DHT dht2(SensorPin,DHT11);
WiFiClientSecure espClient;
CircuitIoTTemperatureSensor temperatureSensor(espClient, token);

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

void setupTemperatureSensor() {  
 delay(10);
 temperatureSensor.setup();
}


void setup() {
  Serial.begin(9600);

  pinMode(SensorPin, INPUT_PULLUP);

  pinMode(wifiLed, OUTPUT);

  //During Starting WiFi LED should TURN OFF
  digitalWrite(wifiLed, HIGH);
  
  
  Serial.println("Connecting..."); 
  setupWifi();
  setupTemperatureSensor();
}

void loop() {
  if (!temperatureSensor.connected()) {
    digitalWrite(wifiLed, HIGH);
  }
  else{
    digitalWrite(wifiLed, LOW);
  }   
  temperatureSensor.run();
  delay(5000);

  if(temperature != dht2.readTemperature()){
    Serial.println("Temperature changed:: ");
    Serial.println(dht2.readTemperature());
    temperature = dht2.readTemperature();
    temperatureSensor.pushTemperature(String(dht2.readTemperature()).c_str());
  }
  if(humidity != dht2.readHumidity()){
    Serial.println("Humidity changed:: ");
    Serial.println(dht2.readHumidity());
    humidity = dht2.readHumidity();
    temperatureSensor.pushHumidity(String(dht2.readHumidity()).c_str());
  }

}