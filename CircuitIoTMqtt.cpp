#include <Arduino.h>
#include <string.h>
//#include <WiFiClientSecure.h>
#include "CircuitIoTMqtt.h"

#include "CircuitIoTConfig.h"

CircuitIoTMqtt::CircuitIoTMqtt(WiFiClientSecure& wifiClient) : PubSubClient() {
  
  #ifdef ESP8266 
         wifiClient.setFingerprint(mqttCertFingerprint);
    #endif 
    #ifdef ESP32   
         wifiClient.setCACert(ROOTCERT);
        //wifiClient.setInsecure();
    #endif
    
  this->_wifiClient = &wifiClient;
  PubSubClient::setClient(wifiClient);
  //PubSubClient::setCallback(onCallback);
  //PubSubClient::setCallback([this] (char* topic, byte* payload, unsigned int length) { this->onCallback(topic, payload, length); });
};

void CircuitIoTMqtt::setupMqtt(const char* clientId, const char* username, const char* password, bool subscribe) {
  setServer(CIRCUITIOT_BROKER_URL, CIRCUITIOT_BROKER_PORT);
  this->_clientId = clientId;
  this->_username = username;
  this->_password = password;
  this->_doSubscribe = subscribe;
  this->_id = "myid";
  this->_rootTopic = String("device/" + _clientId).c_str();

  if (WiFi.status() == WL_CONNECTED) connect();
  //sendLog("I'm born. Hello World!");
  //sendBootNotification();
}

void CircuitIoTMqtt::runMqtt() {
  if (!connected()) {
     if (WiFi.status() == WL_CONNECTED) connect();
     //delay(200);
   }
  loop();
}

bool CircuitIoTMqtt::connect() {
  if (!PubSubClient::connect(_clientId.c_str(), _username.c_str(), _password.c_str())) {
    Serial.printf("!! Error while connecting to the MQTT broker, code: %d\n", this->state());
    return false;
  }else{
    Serial.println("Connection to MQTT broker done :-)");

    //subscribe if applicable
    if(_doSubscribe){
      String topic  = String(_rootTopic + "/#").c_str();
      Serial.printf("Subscribing to MQTT topic: %s", topic.c_str());
      this->subscribe(topic.c_str());
    }

    return true; 
  }    
}

bool CircuitIoTMqtt::reconnect() {
  while (!this->connected()) {
    if (!PubSubClient::connect(_clientId.c_str(), _username.c_str(), _password.c_str())) {
      Serial.printf("!! Error while connecting to the MQTT broker, code: %d\n", this->state());
      // Wait 1 second before retrying
      delay(1000);
      continue;
    }
    Serial.println("Connection to MQTT broker done :-)");

    if(_doSubscribe){
      String topic  = String(_rootTopic + "/#").c_str();
      Serial.printf("Subscribing to MQTT topic: %s", topic.c_str());
      this->subscribe(topic.c_str());
    }
  }
  return true;
}

bool CircuitIoTMqtt::sendMessage(const char *topic, const char *payload) {
  Serial.printf("Sending message '%s' to topic '%s'\n", payload, topic);
  return this->publish(topic, payload);
}

bool CircuitIoTMqtt::sendBootNotification() {
  return sendMessage(String(_rootTopic + "/boot").c_str(), "1");
}

bool CircuitIoTMqtt::sendLog(const char *message) {
  if (!_doSendLogs) return false;
  return sendMessage(String(_rootTopic + "/log").c_str(), message);
}
