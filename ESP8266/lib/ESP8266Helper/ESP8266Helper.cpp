#include "ESP8266Helper.h"

int ESP8266Helper::GetResetReason()
{
    return system_get_rst_info()->reason;
}

bool ESP8266Helper::CheckResetReason(ResetReason Reason)
{
    return ((int)Reason == system_get_rst_info()->reason);
}