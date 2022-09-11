#include <Arduino.h>
#ifdef ESP8266 
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
       #include <WiFi.h>
#endif

#include <CircuitIoTContactSensor.h>

#define SensorPin D2  //D2

//WiFi Status LED
#define wifiLed    D4   //D4

bool isTriggered = false;

//SSID and password
const char* ssid = "";
const char* password = "";

//Device token
char token[] = "";

WiFiClientSecure espClient;
CircuitIoTContactSensor contactSensor(espClient, token);

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

void setupContactSensor() {  
 delay(10);
 contactSensor.setup();
}


void setup() {
  Serial.begin(9600);

  pinMode(SensorPin, INPUT_PULLUP);

  pinMode(wifiLed, OUTPUT);

  //During Starting WiFi LED should TURN OFF
  digitalWrite(wifiLed, HIGH);
  
  
  Serial.println("Connecting..."); 
  setupWifi();
  setupContactSensor();
}

void loop() {
  if (!contactSensor.connected()) {
    digitalWrite(wifiLed, HIGH);
  }
  else{
    digitalWrite(wifiLed, LOW);
  }   
  contactSensor.run();
  delay(1000);
  

   if(digitalRead(SensorPin) == HIGH){
       Serial.println("Open...");
       if (isTriggered==false){
        contactSensor.pushContactStatus("1");
        Serial.println("Published 1");
       }
       isTriggered = true;
   }else{
       Serial.println("Closed...");
       if (isTriggered==true){  
        contactSensor.pushContactStatus("0");
        Serial.println("Published 0");
       }
       isTriggered = false;
   }

}