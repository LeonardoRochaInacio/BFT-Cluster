#include <Arduino.h>
#include <Definitions.h>
#include <Communication.h>

void DefaultUpload_Setup()
{
    Communication::GenerateNetwork();
    Communication::UDPInitialize(2000);
}

void DefaultUpload_Loop()
{
    Communication::UDPReadMessage(2000);
}