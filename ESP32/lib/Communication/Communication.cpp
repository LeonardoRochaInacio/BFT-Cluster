#include "Communication.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <vector>
#include <SleepManager.h>
#include "../../include/Definitions.h"


IPAddress Communication::LasConnectionGateway = IPAddress((uint32_t)0);
WiFiUDP Communication::UDPInstance;
char Communication::LastReceivedPacketContent[255];

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
    LOG("[Wi-Fi] Tentando conexão para: " << ConnectionParams.ConnectionUser << "@" << ConnectionParams.ConnectionPassword);

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
        LOG("[Wi-Fi] Estado da conexão: " << WiFi.status() << " | Tentativa: " << (int)CurrentAttempts); 
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
        SleepManager::SetSleep(TRYING_CONNECTION_SLEEP_TIME, EWakeType::RadioFrequency_Enable);
    }
    else
    {
        LasConnectionGateway = WiFi.gatewayIP();
        LOG("[Wi-Fi] Conectado em: " << ConnectionParams.ConnectionUser);
        LOG("[Wi-Fi] Gateway atual: " << LasConnectionGateway.toString().c_str());
    }

    /*
    * Executa callback de response
    */
    if(ConnectionParams.ConnectionResponseCallback) ConnectionParams.ConnectionResponseCallback(Response);
}

void Communication::GenerateNetwork()
{
    WiFi.persistent(false);
    WiFi.mode(WIFI_OFF);
    WiFi.mode(wifi_mode_t::WIFI_MODE_APSTA);
    WiFi.softAP(MASTER_STATION_NAME, (const char*)__null, 1, 1, MASTER_MAX_CONNECTED_DEVICES);
    LOG("[Wi-Fi] AP iniciado | MAC: " << WiFi.softAPmacAddress().c_str() << " | IP: " << WiFi.softAPIP().toString().c_str());
}

void Communication::UDPInitialize(int Port)
{
    UDPInstance.begin(Port);
}

void Communication::UDPSendMessage(const char* Message, int Port)
{
    if(LasConnectionGateway != IPAddress((uint32_t)0))
    {
        if(UDPInstance.beginPacket(LasConnectionGateway, Port))
        {
            LOG("[UDP] Pacote UDP iniciado com sucesso.");
            UDPInstance.write((uint8_t*)Message, (strlen(Message)*sizeof(char)));
            LOG("[UDP] Enviando pacote UDP: {Message: '" << Message << "', IP: '" << LasConnectionGateway.toString().c_str() << "', Port: '" << Port << "'}");
        }      
        if(UDPInstance.endPacket())
        {
            LOG("[UDP] Pacote UDP enviado com sucesso.");
        }
    }
}

void Communication::UDPReadMessage(int Port)
{ 
    if(GetNumConectedDevices())
    {
        int PacketSize = UDPInstance.parsePacket();
        if (PacketSize)
        {
            LOG("[UDP] Recebido " << PacketSize << " bytes || IP:Porta remetente: " << UDPInstance.remoteIP().toString().c_str() << ":" << UDPInstance.remotePort());

            int Length = UDPInstance.read(LastReceivedPacketContent, 255);
            LastReceivedPacketContent[Length] = '\0';

            LOG("[UDP] Conteúdo do pacote: " << LastReceivedPacketContent);
        }
    }
}

int Communication::GetNumConectedDevices()
{
    wifi_sta_list_t StationList;
    esp_wifi_ap_get_sta_list(&StationList);  
    return StationList.num;
}

/*std::vector<uint8_t[6]> Communication::GetConectedDevices()
{
    wifi_sta_list_t StationList;
    esp_wifi_ap_get_sta_list(&StationList);  
    
    std::vector<uint8_t[6]> StaList;
    
    for(int i = 0; i < StationList.num; i++) {
    
        wifi_sta_info_t Station = StationList.sta[i];
        StaList.push_back(Station.mac);
    }

    return StaList;
}*/