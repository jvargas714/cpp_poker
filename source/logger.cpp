#include "logger.h"


// Global interface
logger& log()
{
#ifdef TESTING
	static logger* lg = new logger( TEST_SUITE_LOG );
#elif POKER_DEBUG
	static logger* lg = new logger( DEBUG_LOG );
#else
	static logger* lg = new logger;
#endif
	return *lg;
}

void delete_logger()
{
	delete &log();
}

std::string now()
{
	return "";
}

/*
std::string now()
{
    std::time_t t = std::time( nullptr );
    std::tm* tm = std::localtime( &t );
    char buff[128];
    std::strftime( buff, sizeof(buff), "%Y-%m-%d.%X ", tm );
    return buff;
}
*/