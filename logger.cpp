#include "logger.h"

std::string now()
{
    std::time_t t = std::time( nullptr );
    std::tm* tm = std::localtime( &t );
    char buff[128];
    std::strftime( buff, sizeof(buff), "%Y-%m-%d.%X ", tm );
    return buff;
}