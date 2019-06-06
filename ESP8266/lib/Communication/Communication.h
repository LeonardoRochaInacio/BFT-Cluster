#include <WiFiUdp.h>

#define MAX_ATTEMPT 30
#define TIME_BETWEEN_ATTEMPT 200

/**
 * @brief Tipos de resposta do metodo TryToConnect
 * 
 */
enum EConnectionResponse
{
    CON_OK,
    CON_ERROR
};

/**
 * @brief Definicoes de tipo para callbacks
 * 
 */
typedef void (*VoidFunc)(void);
typedef void (*ConResponseFunc)(EConnectionResponse);

/**
 * @brief Estrutura para definir conexao
 * É possivel passar callback para ser executado antes e apos a tentativa de conexao
 */
struct FConnectionParams
{
    /**
     * @brief Usuário para conexão
     * 
     */
    const char* ConnectionUser;

    /**
     * @brief Senha para conexão
     * 
     */
    const char* ConnectionPassword;

    /**
     * @brief Função para ser chamada pre conexão
     * 
     */
    VoidFunc PreConnectAttempCallback = nullptr;

    /**
     * @brief Função para ser chamada como resposta da conexão
     * Layout: void function(EConnectionResponse Response);
     */
    ConResponseFunc ConnectionResponseCallback = nullptr;
    
    /**
     * @brief Dita se será usado BSSID para conexão
     * 
     */
    bool UseBSSID = false;

    /**
     * @brief BSSID, só funciona caso UseBSSID estiver true
     * 
     */
    uint8_t* BSSID;
};

/**
 * @brief Classe responssavel pela comunicao do esp8266 ao esp32
 * 
 */
class Communication
{
public:

    /**
     * @brief Instância UDP para manipular mensagens e conexões
     * 
     */
    static WiFiUDP UDPInstance;

    /**
     * @brief Armazena o gateway da última conexão
     * 
     */
    static IPAddress LasConnectionGateway;

    /**
     * @brief Tenta a conexão
     * 
     * @param ConnectionParams 
     */
    static void TryToConnect(struct FConnectionParams ConnectionParams);

    /**
     * @brief Envia uma mensagem UDP
     * 
     */
    static void UDPInitialize(int Port);

    /**
     * @brief Envia uma mensagem UDP
     * 
     */
    static void UDPSendMessage(const char* Message, int Port);

};