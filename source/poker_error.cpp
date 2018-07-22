#include "poker_error.h"

poker_error::HandIDError::HandIDError() :
        runtime_error("Error: Hand Identification Error") {}


poker_error::HandIDError::HandIDError(const std::string &msg) :
        runtime_error(msg) {}


poker_error::FindWinnerError::FindWinnerError() :
        runtime_error("Error: findHand must be run for all players!") {}


poker_error::FindWinnerError::FindWinnerError(const std::string &msg) :
        runtime_error(msg) {}


poker_error::FindPlayerError::FindPlayerError() :
        runtime_error("Error: Player not found") {}


poker_error::FindPlayerError::FindPlayerError(const std::string &msg) :
        runtime_error(msg) {}

poker_error::CfgLoadError::CfgLoadError(const std::string &msg) : runtime_error(msg) {}

poker_error::CfgLoadError::CfgLoadError() :runtime_error("Error loading application configuration") {}
