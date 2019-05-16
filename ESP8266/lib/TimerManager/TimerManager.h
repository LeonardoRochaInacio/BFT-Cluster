#pragma once

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

};