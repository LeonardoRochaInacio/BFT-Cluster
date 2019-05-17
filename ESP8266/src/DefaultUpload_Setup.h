#include <Arduino.h>
#include <Definitions.h>
#include <TimerManager.h>
#include <StorageTypes/ClockData.h>
#include <StorageManager.h>
#include <iostream>
#include <ESP8266Helper.h>

void CycleCount();

static double Temperature = 0;

void DefaultUpload_Setup()
{
    pinMode(A0, INPUT);
    CycleCount();
    
}

void DefaultUpload_Loop()
{
    while(Temperature == 0)
    {
        Temperature = analogRead(A0)*TEMPERATURE_PROPORTION;
        std::cout << "Current temperature: " << Temperature << std::endl;
    }

    TimerManager::SetSleep(SLEEP_TIME);
}

void CycleCount()
{
    ClockData *CLKData = new ClockData();

    std::cout << "Cycle Type: " << ESP8266Helper::GetResetReason() << std::endl;

    if(ESP8266Helper::CheckResetReason(ResetReason::DEEP_SLEEP_EXECUTION))
    {
        StorageManager::RestoreData_RTCMemory<ClockData>(CLKData);
        CLKData->SleepCycles += 1;
        std::cout << "Total sleep cycle counter: " << CLKData->SleepCycles << std::endl;
    }

    StorageManager::StoreData_RTCMemory<ClockData>(CLKData);    
}