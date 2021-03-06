#include <Arduino.h>
#include <Definitions.h>
#include <TimerManager.h>
#include <StorageTypes/ClockData.h>
#include <StorageManager.h>
#include <iostream>
#include <ESP8266Helper.h>
#include <Communication.h>
#include <string>

/**
 * @brief Última temperatura registrada
 * 
 */
static float LastTemperature; 

/**
 * @brief Executa o contador de ciclos, usado para operações de sincronização
 * 
 */
void CycleCount();

/**
 * @brief Retorno para TryToConnect
 * 
 * @param Rsp Resposta proveniente de TryToConnect passada em FConnectionParams
 */
void ConnectionResponse(EConnectionResponse Rsp);

/**
 * @brief Função setup padrão
 * 
 */
void DefaultUpload_Setup()
{
    /*
    * Habilita pino para leitura analogica do sensor.
    */
    pinMode(A0, INPUT);
    Communication::UDPInitialize(2000);
    //CycleCount();
}

/**
 * @brief Função loop padrão
 * 
 */
void DefaultUpload_Loop()
{

    FConnectionParams ConnectionParams;
    ConnectionParams.ConnectionUser = "ESP32_01";
    ConnectionParams.ConnectionPassword = "";
    ConnectionParams.ConnectionResponseCallback = ConnectionResponse;
    ConnectionParams.UseBSSID = true;
    ConnectionParams.BSSID = (uint8_t*)"\x30\xAE\xA4\x05\x1F\xED";
    Communication::TryToConnect(ConnectionParams);
}

void CycleCount()
{
    ClockData *CLKData = new ClockData();

    LOG("Cycle Type: " << ESP8266Helper::GetResetReason());

    if(ESP8266Helper::CheckResetReason(ResetReason::DEEP_SLEEP_EXECUTION))
    {
        StorageManager::RestoreData_RTCMemory<ClockData>(CLKData);
        CLKData->SleepCycles += 1;
        LOG("Total sleep cycle counter: " << CLKData->SleepCycles);
    }

    StorageManager::StoreData_RTCMemory<ClockData>(CLKData);    
}

/**
 * @brief Funcao que é passada no TryToConnect
 * 
 * @param Rsp Resposta da conexao
 */
void ConnectionResponse(EConnectionResponse Rsp)
{
    if(Rsp == EConnectionResponse::CON_OK)
    {
        LastTemperature = analogRead(A0)*TEMPERATURE_PROPORTION;
        LOG("Temperatura atual: " << LastTemperature);
        char LastTemperature_Char[12];
        sprintf(LastTemperature_Char, "%f", LastTemperature);
        Communication::UDPSendMessage(LastTemperature_Char,2000);
        /**
         * @brief Delay extremamente importante, caso não exista, a antena se desligará antes mesmo de transmitir os bytes empacotados no UDP
         */
        delay(100);
        TimerManager::SetSleep(SLEEP_TIME, EWakeType::RadioFrequency_Enable);
    }
}