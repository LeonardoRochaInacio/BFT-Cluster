#pragma once
#include <esp_attr.h>

/**
 * @brief Possiveis modos para o ESP acordar de um deepsleep
 * 
 */
enum EWakeType
{
    RadioFrequency_Enable = 0, //Inicia com o transmissor/receptor WiFi ligado
    RadioFrequency_Disable = 4 //Inicia com o transmissor/receptor WiFi desligado
};

/**
 * @brief Classe que gerencia o relógio atual e os disparos de funcoes.
 * 
 */
class SleepManager
{

public:

    /**
     * @brief Construtor padrão
     * 
     */
    SleepManager();

     /**
     * @brief Retorna os quantos ciclos ja se passaram
     * 
     */
    static int GetCycles();

     /**
     * @brief Conta um ciclo
     * 
     */
    static void CountCycle();

    /**
     * @brief Coloca o esp8266 em modo sleep por x segundos
     * 
     */
    static void SetSleep(int Seconds, EWakeType WakeType);

private:
    /**
     * @brief Armazena ciclos contatos
     * 
     */
    static RTC_DATA_ATTR int Cycles;
};