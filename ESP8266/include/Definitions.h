#pragma once
#include <iostream>

#define DEBUG_MODE 1
#define TEMPERATURE_PROPORTION 0.322265625f
#define TRYING_CONNECTION_SLEEP_TIME 5
#define SLEEP_TIME 60
#define SENSOR_READ_DELAY 200

#define LOG(X) if(DEBUG_MODE) std::cout << X << std::endl