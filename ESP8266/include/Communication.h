#include <Definitions.h>

#if DEBUG_MODE
#include <iostream>
#endif

#include <ESP8266WiFi.h>
#include <TimerManager.h>
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
    const char* ConnectionUser;
    const char* ConnectionPassword;
    VoidFunc PreConnectAttempCallback = nullptr;
    ConResponseFunc ConnectionResponseCallback = nullptr;
};

/**
 * @brief Classe responssavel pela comunicao do esp8266 ao esp32
 * 
 */
class Communication
{
public:

    static void TryToConnect(FConnectionParams ConnectionParams)
    {
        #if DEBUG_MODE
            std::cout << "Tentando conexão para: " << ConnectionParams.ConnectionUser << "@" << ConnectionParams.ConnectionPassword << std::endl;
        #endif
        if(ConnectionParams.PreConnectAttempCallback) ConnectionParams.PreConnectAttempCallback();
        WiFi.mode(WiFiMode::WIFI_STA);
        WiFi.begin(ConnectionParams.ConnectionUser, ConnectionParams.ConnectionPassword);
        EConnectionResponse Response = (WiFi.status() != WL_CONNECTED) ? EConnectionResponse::CON_ERROR : EConnectionResponse::CON_OK;
        if(ConnectionParams.ConnectionResponseCallback) ConnectionParams.ConnectionResponseCallback(Response);
        if(Response != EConnectionResponse::CON_OK)
        {
            TimerManager::SetSleep(TRYING_CONNECTION_SLEEP_TIME, EWakeType::RadioFrequency_Enable);
        }
        else
        {
            #if DEBUG_MODE
                std::cout << "Conectado em: " << ConnectionParams.ConnectionUser << "@" << ConnectionParams.ConnectionPassword << std::endl;
            #endif
        }
    }
};