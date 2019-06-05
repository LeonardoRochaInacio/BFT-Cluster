#include <Arduino.h>
#include <Definitions.h>
#include <esp_sleep.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <WiFiUdp.h>

void PrintStations()
{
  wifi_sta_list_t stationList;
 
  esp_wifi_ap_get_sta_list(&stationList);  
 
  Serial.print("N of connected stations: ");
  Serial.println(stationList.num);
 
  for(int i = 0; i < stationList.num; i++) {
 
    wifi_sta_info_t station = stationList.sta[i];
 
    for(int j = 0; j< 6; j++){
      char str[3];
 
      sprintf(str, "%02x", (int)station.mac[j]);
      Serial.print(str);
 
      if(j<5){
        Serial.print(":");
      }
    }
    Serial.println();
  }
 
  Serial.println("-----------------");
}

static WiFiUDP UDPInstance;
static char incomingPacket[256];

void DefaultUpload_Setup()
{
    WiFi.mode(wifi_mode_t::WIFI_MODE_APSTA);
    WiFi.softAP(MASTER_STATION_NAME);
    UDPInstance.begin(2000);
    LOG("ESP32 @ MAC: " << WiFi.softAPmacAddress().c_str());
    LOG("ESP32 @ IP: " << WiFi.softAPIP().toString().c_str());
}

void DefaultUpload_Loop()
{
    PrintStations();  
    int packetSize = UDPInstance.parsePacket();
    if (packetSize)
    {
      Serial.printf("Received %d bytes from %s, port %d\n", packetSize, UDPInstance.remoteIP().toString().c_str(), UDPInstance.remotePort());
      int len = UDPInstance.read(incomingPacket, 255);
      if (len > 0)
      {
        incomingPacket[len] = '\0';
      }
      Serial.printf("UDP packet contents: %s\n", incomingPacket);
    }
    delay(250);
}