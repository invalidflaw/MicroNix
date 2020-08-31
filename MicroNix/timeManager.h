#pragma once
#include "main.h"

// experimental external calls
extern IPAddress gtimeServer;
extern WiFiUDP gudp;
extern DateTime gnow;
extern MCP7940_Class grtc;
extern int gtimeZone;
extern bool gdstMode;

class timeManager
{
  public:
    timeManager();

  public:
    
    void get();

    bool sync();

  private:
    void sendNTPpacket();
    void dstCheck();
    
  private:
    bool wifiSynced = false;
    byte packetBuffer[NTP_PACKET_SIZE];
    unsigned long lastSync = 0;

  // Reference Objects
  private:
//    IPAddress& address;
//    WiFiUDP& udp;
//    DateTime& now;
//    MCP7940_Class& rtc;
//    int& timeZone;
//    bool& dstMode;

};
