#include <Arduino.h>
#ifdef ESP8266 
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
       #include <WiFi.h>
#endif

#include <CircuitIoTSwitch.h>

//SSID and password
const char* ssid = "";
const char* password = "";

//Device token
char token[] = "";

WiFiClientSecure espClient;
CircuitIoTSwitch powerSwitch(espClient, token);

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


void powerStateChangeCallback(const char* value) {
  Serial.println(".");
  Serial.printf("Power state received: [%s] ", value);
  Serial.println(".");
}

void setupSwitch() {  
 delay(10);
 powerSwitch.setup();
 powerSwitch.onPowerStateChange(powerStateChangeCallback);
}


void setup() {
  Serial.begin(9600);
  Serial.println("Connecting..."); 
  setupWifi();
  setupSwitch();
}

void loop() {
  powerSwitch.run();
  delay(500);

}