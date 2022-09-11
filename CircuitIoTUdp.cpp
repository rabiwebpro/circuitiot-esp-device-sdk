#include "CircuitIoTUdp.h"

CircuitIoTUdp::CircuitIoTUdp() {};

void CircuitIoTUdp::setupUdp() {
  _udp.begin(UDP_PORT);
  Serial.print("Listening on UDP port ");
  Serial.println(UDP_PORT);
}

void CircuitIoTUdp::runUdp() {
  int packetSize = _udp.parsePacket();
  if (packetSize) {
    int len = _udp.read(_packet, 255);
    if (len > 0)
    {
      _packet[len] = '\0';
    }
    processPacket(_packet);

    char reply[] = "ok";
    _udp.beginPacket(_udp.remoteIP(), _udp.remotePort());
    #if defined(ESP8266) 
         _udp.write(reply);
    #endif 
    #if defined(ESP32)   
         int i = 0;
         while (reply[i] != 0) _udp.write((uint8_t)reply[i++]);
    #endif
    _udp.endPacket();
  }
}

void CircuitIoTUdp::processPacket(char packet[]){
  String payload = String(packet);
  if(payload.indexOf("|") > 0){
    char *strings[2];
    char *ptr = NULL;
    byte index = 0;
    char* np = (char *) payload.c_str();
    ptr = strtok(np, "|");
    while (ptr != NULL)
     {
        strings[index] = ptr;
        index++;
        ptr = strtok(NULL, "|");
     }
     
     if (udpCallback != NULL){
      udpCallback(strings[0], (uint8_t*)strings[1], sizeof(strings[1]));
     }        
  }
}

void CircuitIoTUdp::setUdpCallback(UDP_CALLBACK_SIGNATURE) {
    this->udpCallback = udpCallback;
}
