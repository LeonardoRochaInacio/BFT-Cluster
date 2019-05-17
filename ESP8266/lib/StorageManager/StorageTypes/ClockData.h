#pragma once
/**
 * @brief Classe para ser usada estaticamente para armazenamento de dados na EEPROM
 * Basta inserir a propriedade, a classe inteira sera armazenada binariamente e depois castada no ato da recuperacao de dados.
 */

class ClockData
{
public:

    /**
     * @brief Palavra chave utilizada na checagem de validade dos dados pra recuperacao dos mesmos
     * Pode ser substituida por metodos de RST REASON
     */
    char Secret[7] = "9c11dk";

    /**
     * @brief Armazena tempo imediatamente antes da inicializacacao de um deep sleep
     * 
     */
    long SleepCycles = 0;

};