#include "logger.h"


// Global interface
logger &log() {
    static logger *lg;
    if (!lg) {
#ifdef TESTING
        lg = new logger( TEST_SUITE_LOG );
#elif POKER_DEBUG
        lg = new logger( DEBUG_LOG );
#else
        lg = new logger;
#endif
    }
    return *lg;
}

void delete_logger() {
    delete &log();
}

std::string now() {
    return "";
}
