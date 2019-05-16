#include <Arduino.h>
#include <TimerManager.h>
#include <StorageTypes/ClockData.h>
#include <StorageManager.h>
#include <iostream>
#include <ESP8266Helper.h>

#define SLEEPTIME_IN_SECONDS 5

void DefaultUpload_Setup()
{
    pinMode(D1, OUTPUT);
    digitalWrite(D1, HIGH);

    ClockData *CLKData = new ClockData();

    if(ESP8266Helper::CheckResetReason(ResetReason::DEEP_SLEEP_EXECUTION))
    {
        StorageManager::RestoreData_RTCMemory<ClockData>(CLKData);
        printf("REASON: DEEPSLEEP");
        CLKData->SleepCycles += 1;
    }
    else
    {
        printf("REASON: OTHER"); 
    }
    
    StorageManager::StoreData_RTCMemory<ClockData>(CLKData);    

    std::cout << "CurrentTime: " << (double)(CLKData->SleepCycles / 1000.f) << std::endl;
    ESP.deepSleep(SLEEPTIME_IN_SECONDS*(1000000), WAKE_NO_RFCAL);
}

void DefaultUpload_Loop()
{
    
}