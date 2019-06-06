#include <SleepManager.h>
#include <Arduino.h>
#include <sys/time.h>
#include "../../include/Definitions.h"

SleepManager::SleepManager(){}

void SleepManager::SetSleep(int Seconds, EWakeType WakeType)
{
    LOG("[SM] Entrando em modo de deep-sleep por: " << Seconds);
    esp_sleep_enable_timer_wakeup(Seconds*(1000000));
    esp_deep_sleep_start();
}

int SleepManager::GetCycles()
{
    return Cycles;
}

void SleepManager::CountCycle()
{
    Cycles++;
}
