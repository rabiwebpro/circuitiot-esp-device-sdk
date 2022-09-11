#ifndef _CIRCUITIOT_MQTT_H_
#define _CIRCUITIOT_MQTT_H_

#include <Arduino.h>
#ifdef ESP8266 
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
       #include <WiFi.h>
#endif
#include <WiFiClientSecure.h>
#include <PubSubClient.h>



#define MQTT_INVENTORY_TOPIC "house/hardware/ping_all"
#define MQTT_ROOT "house/hardware"
#define MQTT_SENSOR_ROOT "house/sensors/"

class CircuitIoTMqtt : public PubSubClient {
  public:
    CircuitIoTMqtt(WiFiClientSecure& wifiClient);
    // to be called within Arduino setup() global function
    void setupMqtt(const char* clientId, const char* username, const char* password, bool subscribe);
    bool reconnect();
    bool connect();
    void runMqtt();
    bool sendLog(const char *message);
    String _rootTopic;

  private:
    WiFiClientSecure* _wifiClient;
    String _clientId;
    String _username;
    String _password;
    //String _rootTopic;
    String _id;
    bool _doSubscribe = false;
    bool _doSendLogs = false; 
    //bool connect();
    bool sendBootNotification();

  protected:
    bool sendMessage(const char *topic, const char *payload);
};

#endif
