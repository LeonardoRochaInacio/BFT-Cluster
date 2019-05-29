#pragma once

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
class TimerManager
{

public:

    /**
     * @brief Construtor padrão
     * 
     */
    TimerManager();

    /**
     * @brief  Seta o tempo atual do RTC (Real time clock) interno do NodeMCU.
     * O RTC é capaz de ser executado durante um período de deep sleep
     * @param Timestamp_Value Valor em formato timestamp a ser atribuido no RTC
     */
    static void SetRTCTime(long Timestamp_Value);

    /**
     * @brief Retorna o tempo atual do RTC no formato timestamp
     *  
     * @return int Retorno no formato informado na descrição
     */
    static long GetCurrentRTCTime();

    /**
     * @brief Coloca o esp8266 em modo sleep por x segundos
     * 
     */
    static void SetSleep(int Seconds, EWakeType WakeType);

};