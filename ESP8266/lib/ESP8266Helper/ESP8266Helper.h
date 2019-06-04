#pragma once
#include <iostream>
#include "user_interface.h"

/**
 * @brief Razoes disponiveis para inicializacao e restart do esp8266
 * 
 */
enum ResetReason
{
    FIRST_EXECUTION = 6,
    DEEP_SLEEP_EXECUTION = 5
};

/**
 * @brief Helper para funcoes gerais do esp8266 usadas no projeto.
 * 
 */
class ESP8266Helper
{
public:

    /**
     * @brief Pega numericamente a razao da inicializacao ou reset do esp8266
     * 
     * @return int 
     */
    static int GetResetReason();

    /**
     * @brief Faz a checagem se a inicializacao foi igual a razao passada por parametro
     * 
     * @param Reason 
     * @return true 
     * @return false 
     */
    static bool CheckResetReason(ResetReason Reason);
    
};