#include <Arduino.h>
#ifdef ESP8266 
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
       #include <WiFi.h>
#endif

#include <CircuitIoTMultiSwitch.h>

#define RelayPin1 D1 
#define RelayPin2 D2 
#define RelayPin3 D5 
#define RelayPin4 D6 

#define SwitchPin1 D7 
#define SwitchPin2 1  
#define SwitchPin3 D3 
#define SwitchPin4 3  

//WiFi Status LED
#define wifiLed    D4 

int toggleState_1 = 1;
int toggleState_2 = 1;
int toggleState_3 = 1;
int toggleState_4 = 1;

//SSID and password
const char* ssid = "";
const char* password = "";

//Device token
char token[] = "";

WiFiClientSecure espClient;
CircuitIoTMultiSwitch powerSwitch(espClient, token);

void setupWifi() {
 delay(10);
 WiFi.begin(ssid, password);
 delay(3000);

randomSeed(micros());
}


void powerStateChangeCallback(int deviceIndex, const char* value) {

  if(deviceIndex == 1){
    if (strcmp(value, "1") == 0){
      Serial.println("device 1 on");
      digitalWrite(RelayPin1, LOW);
    }else{
      Serial.println("device 1 off");
      digitalWrite(RelayPin1, HIGH);
    }
  }
  if(deviceIndex == 2){
    if (strcmp(value, "1") == 0){
      digitalWrite(RelayPin2, LOW);
    }else{
      digitalWrite(RelayPin2, HIGH);
    }
  }
  if(deviceIndex == 3){
    if (strcmp(value, "1") == 0){
      digitalWrite(RelayPin3, LOW);
    }else{
      digitalWrite(RelayPin3, HIGH);
    }
  }
  if(deviceIndex == 4){
    if (strcmp(value, "1") == 0){
      digitalWrite(RelayPin4, LOW);
    }else{
      digitalWrite(RelayPin4, HIGH);
    }
  }
  
}

void setupSwitch() {  
 delay(10);
 powerSwitch.setup();
 powerSwitch.onPowerStateChange(powerStateChangeCallback);
}

void manualControl(){
    //Manual Switch 1
    if (digitalRead(SwitchPin1) == LOW){
      delay(100); 
      if(toggleState_1 == 0){
        digitalWrite(RelayPin1, LOW);
        toggleState_1 = 1;
        if (powerSwitch.connected()) powerSwitch.pushPowerStateChange(1, "1");
      }
    }else{
      delay(100);
      if(toggleState_1 == 1){
        digitalWrite(RelayPin1, HIGH);
        toggleState_1 = 0;
        if (powerSwitch.connected()) powerSwitch.pushPowerStateChange(1, "0");
      }
    }
    //Manual Switch 2
    if (digitalRead(SwitchPin2) == LOW){
      delay(100); 
      if(toggleState_2 == 0){
        digitalWrite(RelayPin2, LOW);
        toggleState_2 = 1;
        if (powerSwitch.connected()) powerSwitch.pushPowerStateChange(2, "1");
      }
    }else{
      delay(100);
      if(toggleState_2 == 1){
        digitalWrite(RelayPin2, HIGH);
        toggleState_2 = 0;
        if (powerSwitch.connected()) powerSwitch.pushPowerStateChange(2, "0");
      }
    }
    //Manual Switch 3
    if (digitalRead(SwitchPin3) == LOW){
      delay(100); 
      if(toggleState_3 == 0){
        digitalWrite(RelayPin3, LOW);
        toggleState_3 = 1;
        if (powerSwitch.connected()) powerSwitch.pushPowerStateChange(3, "1");
      }
    }else{
      delay(100);
      if(toggleState_3 == 1){
        digitalWrite(RelayPin3, HIGH);
        toggleState_3 = 0;
        powerSwitch.pushPowerStateChange(3, "0");
      }
    }
    //Manual Switch 4
    if (digitalRead(SwitchPin4) == LOW){
      delay(100); 
      if(toggleState_4 == 0){
        digitalWrite(RelayPin4, LOW);
        toggleState_4 = 1;
        if (powerSwitch.connected()) powerSwitch.pushPowerStateChange(4, "1");
      }
    }else{
      delay(100);
      if(toggleState_4 == 1){
        digitalWrite(RelayPin4, HIGH);
        toggleState_4 = 0;
        if (powerSwitch.connected()) powerSwitch.pushPowerStateChange(4, "0");
      }
    }
    delay(100);
}



void setup() {
  Serial.begin(9600);

  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);

  pinMode(SwitchPin1, INPUT_PULLUP);
  pinMode(SwitchPin2, INPUT_PULLUP);
  pinMode(SwitchPin3, INPUT_PULLUP);
  pinMode(SwitchPin4, INPUT_PULLUP);

  pinMode(wifiLed, OUTPUT);

  //During Starting all Relays should TURN OFF
  digitalWrite(RelayPin1, HIGH);
  digitalWrite(RelayPin2, HIGH);
  digitalWrite(RelayPin3, HIGH);
  digitalWrite(RelayPin4, HIGH);

  //During Starting WiFi LED should TURN OFF
  digitalWrite(wifiLed, HIGH);
  
  
  Serial.println("Connecting..."); 
  setupWifi();
  setupSwitch();
}

void loop() {
  if (!powerSwitch.connected()) {
    digitalWrite(wifiLed, HIGH);
  }
  else{
    digitalWrite(wifiLed, LOW);
  }
  manualControl();  
  powerSwitch.run();
  delay(100);   

}