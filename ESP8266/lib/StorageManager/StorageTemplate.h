#include <string>

/**
 * @brief Classe helper para ser usada estaticamente para armazenamento de dados na EEPROM
 * Basta inserir a propriedade, a classe inteira sera armazenada binariamente e depois castada no ato da recuperacao de dados.
 */
class StorageTemplate
{
public:

    /**
     * @brief String Propertie Template
     * 
     */
    std::string StringTemplate;

     /**
     * @brief Int Propertie Template
     * 
     */
    int IntTemplate;

};