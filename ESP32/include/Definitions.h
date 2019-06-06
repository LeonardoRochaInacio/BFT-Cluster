#pragma once
#include <iostream>

#define Stringfy(x) #x

#define DEBUG_MODE 1
#define MASTER_STATION_ID "1"
#define MASTER_STATION_NAME "ESP32_0" MASTER_STATION_ID
#define MASTER_MAX_CONNECTED_DEVICES 4
#define TRYING_CONNECTION_SLEEP_TIME 5

#define LOG(X) if(DEBUG_MODE) std::cout << X << std::endl