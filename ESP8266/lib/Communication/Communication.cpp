#include "Communication.h"
#include <ESP8266WiFi.h>
#include <TimerManager.h>
#include "../../include/Definitions.h"

IPAddress Communication::LasConnectionGateway = IPAddress(0);
WiFiUDP Communication::UDPInstance;

void Communication::TryToConnect(struct FConnectionParams ConnectionParams)
{
        /*
         * Checa se já está conectado
         */
        if(WiFi.status() == WL_CONNECTED)
        {
            return;
        }

        /*
        * Variável auxiliar para quantidade de tentativas atual após o WiFi.begin
        */
        int8_t CurrentAttempts = 1;

        /*
        * Inicia uma tentativa, caso nao esteja conectado ainda
        */
        LOG("Tentando conexão para: " << ConnectionParams.ConnectionUser << "@" << ConnectionParams.ConnectionPassword);

        /*
        *  Executa callback de pre conexao
        */
        if(ConnectionParams.PreConnectAttempCallback) ConnectionParams.PreConnectAttempCallback();

        /*
        * Procedimentos para iniciar conexao na rede dos parametros passados
        * WiFi.persistent(false) - Extremamente importante para evitar que a memória flash se corrompa
        */
        WiFi.persistent(false);
        WiFi.mode(WIFI_OFF);
        WiFi.mode(WIFI_STA);

        /**
         * @brief Caso a um BSSID seja passado no parâmetro, haverá conexão direta a ele
         * Caso não, a conexão partirá do SSID com scan, (operação)
         */
        if(ConnectionParams.UseBSSID)
        {
            WiFi.begin(ConnectionParams.ConnectionUser, ConnectionParams.ConnectionPassword, 1, ConnectionParams.BSSID);
        }
        else
        {
            WiFi.begin(ConnectionParams.ConnectionUser, ConnectionParams.ConnectionPassword);
        }
        
        /*
        * Checa se os estados sao IDLE ou DISCONNECTED, caso sim, mantem em loop até o begin dar um resultado satisfatório
        * Caso não dê, o numero de tentativas irá exceder, e avançará no código
        */
        while((WiFi.status() == WL_IDLE_STATUS || WiFi.status() == WL_DISCONNECTED) && CurrentAttempts <= MAX_ATTEMPT)
        {
            LOG("Estado da conexão: " << WiFi.status() << " | Tentativa: " << (int)CurrentAttempts); 
            delay(TIME_BETWEEN_ATTEMPT);
            CurrentAttempts++;
        }

        /*
        * Cria um response para abstrair nos callbacks
        */
        EConnectionResponse Response = (WiFi.status() != WL_CONNECTED) ? EConnectionResponse::CON_ERROR : EConnectionResponse::CON_OK;
        
        /*
        * Caso a conexão não tenha resposta positiva, entra em sleep.
        */
        if(Response != EConnectionResponse::CON_OK)
        {
            TimerManager::SetSleep(TRYING_CONNECTION_SLEEP_TIME, EWakeType::RadioFrequency_Enable);
        }
        else
        {
            LasConnectionGateway = WiFi.gatewayIP();
            LOG("Conectado em: " << ConnectionParams.ConnectionUser);
            LOG("Gateway atual: " << LasConnectionGateway.toString().c_str());
        }

        /*
        * Executa callback de response
        */
        if(ConnectionParams.ConnectionResponseCallback) ConnectionParams.ConnectionResponseCallback(Response);
    }

    void Communication::UDPInitialize(int Port)
    {
        UDPInstance.begin(Port);
    }

    void Communication::UDPSendMessage(const char* Message, int Port)
    {
        if(LasConnectionGateway != IPAddress(0))
        {
            if(UDPInstance.beginPacket(LasConnectionGateway, Port))
            {
                LOG("Pacote UDP iniciado com sucesso.");
                UDPInstance.write(Message, strlen(Message)*sizeof(char));
                LOG("Enviando pacote UDP: {Message: '" << Message << "', IP: '" << LasConnectionGateway.toString().c_str() << "', Port: '" << Port << "'}");
            }      
            if(UDPInstance.endPacket())
            {
                LOG("Pacote UDP enviado com sucesso.");
            }
        }
    }