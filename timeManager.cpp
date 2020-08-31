#include "timeManager.h"

timeManager::timeManager()
{
  lastSync = gnow.unixtime();
}

void timeManager::get()
{
//  Serial.println("RTC Time update"); 
//  gnow = rtc.now(); // THIS CALL HANGS, the assignment to "now" caused this hang, passed next line
  gnow = grtc.now();

//  Serial.println("checkWifiSync");  // this hung
  if(wifiSynced)
  {
//    Serial.println("syncCheck passed");
    if(gnow.hour() == 3 && gnow.unixtime() - lastSync > 28800) // time is 3am or pm and it has been more than 3 hours since last sync
    {
      Serial.println("timed sync passed");
      wifiSynced = sync();
    }
  }
  else if(gnow.unixtime() - lastSync > 600)
  {
    Serial.println("Disconnect Sync");
    wifiSynced = sync();
  }
}

bool timeManager::sync()
{
  Serial.println("check status");
  if(WiFi.status() == 3)
  {
    Serial.println("sending packet");
    sendNTPpacket();
    Serial.println("sent packet");
    
    delay(1000);

    if(gudp.parsePacket())
    {
      Serial.println("i found a packet");
      gudp.read(packetBuffer, NTP_PACKET_SIZE);

      // seperate time data from packet buffer
      unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
      unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);

      // convert to the NTP time
      unsigned long secsSince1900 = highWord << 16 | lowWord;

      // convert to UNIX time
      const unsigned long seventyYears = 2208988800UL;
      unsigned long unixTime = secsSince1900 - seventyYears;

      // convert the unix time to the time zone
      unixTime += (gtimeZone * 3600);

      // adjust for daylight savings time if needed.
      if(gdstMode)
      {
        unixTime -= 3600;
      }

      // adjust the RTC and set the synced state
      grtc.adjust(DateTime(unixTime));
      wifiSynced = true;
      return true;
    }
    else
    {
      // wifi sync failed, set to check periodially
      wifiSynced = false;
      return false;
    }
  }
  return false;
}

void timeManager::sendNTPpacket()
{
  // Clear NTP packet buffer
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // send NTP packets to port 123
  Serial.println("4");
  gudp.beginPacket(gtimeServer, 123);
  Serial.println("5");
  gudp.write(packetBuffer, NTP_PACKET_SIZE);
  Serial.println("6");
  gudp.endPacket();
  Serial.println("udp sent out");
}

void timeManager::dstCheck()
{
  
}
