#pragma once
#include <Arduino.h>
#include "EEPROM.h"
#include "user_interface.h"
/**
 * @brief Macro auxiliar para sizeof
 * 
 */
#define TYPE_SIZE(Type) (sizeof(Type))

/**
 * @brief Classe que gerencia o armazenamento na eeprom
 * Não utilizar alocação dinâmica de memória em tipos passados a esta classe.
 */
class StorageManager
{
public:

    /**
     * @brief Construtor padrão
     * 
     */
    StorageManager();
    
    /**
     * @brief Armazena dado na eeprom
     * 
     * @tparam StorageType Tipo para ser armazenado, tipicamente usar classe Storage
     * @param Instance Instancia de Storage
     * @param OffsetAddress Offset, para caso queira armazenar mais de uma instancia
     */
    template<typename StorageType>
    static void StoreData_EEPROM(StorageType *Instance, int OffsetAddress = 0);

    /**
     * @brief Le a eeprom e converte no tipo.
     * 
     * @tparam StorageType 
     */
    template<typename StorageType>
    static void RestoreData_EEPROM(StorageType *Instance, int OffsetAddress = 0);

    /**
     * @brief Inicia a eepro com o tamanho de bytes do tipo
     * 
     */
    template<typename StorageType>
    static void Begin_EEPROM();

    /**
     * @brief Executa o commit para a eeprom, efetiva os dados.
     * 
     */
    static void End_EEPROM();

    /**
     * @brief Armazena dado na memória RTC
     * 
     * @tparam StorageType Tipo para ser armazenado, tipicamente usar classe Storage
     * @param Instance Instancia de Storage
     * @param OffsetAddress Offset, para caso queira armazenar mais de uma instancia
     */
    template<typename StorageType>
    static void StoreData_RTCMemory(StorageType *Instance, int OffsetAddress = 0);

    /**
     * @brief Le a memória RTC e converte no tipo.
     * 
     * @tparam StorageType 
     */
    template<typename StorageType>
    static void RestoreData_RTCMemory(StorageType *Instance, int OffsetAddress = 0);
};

template<typename StorageType>
void StorageManager::StoreData_EEPROM(StorageType *Instance, int OffsetAddress)
{
    uint8_t* ToBytesInstance = (uint8_t*)Instance;
    int i = 0;
    while(i < TYPE_SIZE(StorageType))
    {
        EEPROM.write(OffsetAddress + i, *(ToBytesInstance + i));
        i++;
    }
}

template<typename StorageType>
void StorageManager::RestoreData_EEPROM(StorageType *Instance, int OffsetAddress)
{
    uint8_t* ToBytesInstance = (uint8_t*)Instance;
    int i = 0;
    while(i < TYPE_SIZE(StorageType))
    {
        *(ToBytesInstance + i) = EEPROM.read(OffsetAddress + i);
        i++;
    }
}

template<typename StorageType>
void StorageManager::Begin_EEPROM()
{
    EEPROM.begin(TYPE_SIZE(StorageType));
}

template<typename StorageType>
void StorageManager::StoreData_RTCMemory(StorageType *Instance, int OffsetAddress)
{
    ESP.rtcUserMemoryWrite(OffsetAddress, (uint32_t*)Instance, sizeof(StorageType));

}

template<typename StorageType>
void StorageManager::RestoreData_RTCMemory(StorageType *Instance, int OffsetAddress)
{
    ESP.rtcUserMemoryRead(OffsetAddress, (uint32_t*)Instance, sizeof(StorageType));
}
