#include <Arduino.h>
#include <iostream>
#include <esp_sleep.h>
#include <WiFi.h>
#include <esp_wifi.h>

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

void DefaultUpload_Setup()
{
    WiFi.mode(wifi_mode_t::WIFI_MODE_APSTA);
    WiFi.softAP("ESP32_01");
    Serial.println(WiFi.softAPIP());
}

void DefaultUpload_Loop()
{
    PrintStations();  
    delay(2000);
}