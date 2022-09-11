#ifndef _CIRCUITIOT_UDP_H_
#define _CIRCUITIOT_UDP_H_

#ifdef ESP8266 
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
       #include <WiFi.h>
#endif
#include <WiFiUdp.h>

#define UDP_PORT 4210

#if defined(ESP8266) || defined(ESP32)
#include <functional>
#define UDP_CALLBACK_SIGNATURE std::function<void(char*, uint8_t*, unsigned int)> udpCallback
#else
#define UDP_CALLBACK_SIGNATURE void (*udpCallback)(char*, uint8_t*, unsigned int)
#endif

class CircuitIoTUdp {
  public:
    CircuitIoTUdp();
    void setupUdp();
    void runUdp();
    void setUdpCallback(UDP_CALLBACK_SIGNATURE);

  private:
    WiFiUDP _udp;
    char _packet[255];
    void processPacket(char packet[]);
    UDP_CALLBACK_SIGNATURE;
};

#endif
