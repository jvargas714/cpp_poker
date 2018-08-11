#include "logger.h"

/*
 * todo :: test various overloads of the logger
 * todo :: vary types of data types and number of data types passed to the logger
 * todo :: test different modules as well
 * todo :: make test4 a for loop putting a few thousand messages in make them random
 * todo :: logger's other functionality as well
 * todo :: logger test to test level enforcement
 */
const std::string LOG_TESTING_FILENAME = "LOGGER_TESTING.log";

 // basic log statements
void basicPrints() {
     LOG_TRACE(LOG_MODULE::TEST);
     LOG_INFO(LOG_MODULE::TEST, "turning off cout printing now...");
     // turn off cout logging
     logger::get_instance().set_std_out(false);
    LOG_INFO(LOG_MODULE::TEXASHOLDEM, "testing testing ", 123, 123);
    LOG_DEBUG(LOG_MODULE::GAME_SERVER,
            "testing gameserver logger!!", "--", "", 1, 1, 1, 2, 2, 3, 4, 5, 65, 6, 7, 8, 9);
}

// try printing containers out from the logger
void stlContainerTesting() {
    LOG_TRACE(LOG_MODULE::TEST);
}

// << operator testing for structs and the logger
void strmOpTesting() {
    LOG_TRACE(LOG_MODULE::TEST);
}

// level enforcement
void lvlEnforcer() {
    LOG_TRACE(LOG_MODULE::TEST);
}

// for loop in messages
void stressTest() {
    LOG_TRACE(LOG_MODULE::TEST);
}

int main() {
    logger::get_instance(LOG_TESTING_FILENAME).log(LOG_MODULE::TEST,
            LOG_LEVEL::INFO,
            __FUNCTION__,
            "*-*-*-*-*-*-*-**-*-*-*-*-*-*-**-*-*-*-*-*-*-*BEGINNING LOG TESTING!!!!!*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*");
    basicPrints();
    stlContainerTesting();
    strmOpTesting();
    lvlEnforcer();
    stressTest();
    logger::get_instance().stopLogging();
    g_QProcThread.join();
    return 0;
}