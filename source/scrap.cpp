#include <iostream>
#include <set>
#include <ctime>
#include <sstream>
#include "logger.h"

using namespace std;

class TestSingleton {
    static TestSingleton* tst;
public:
    TestSingleton()=default;
    static bool alive;
    static TestSingleton* get_instance() {
        if (!TestSingleton::alive)
            TestSingleton::tst = new TestSingleton;
        return TestSingleton::tst;
    }
    void doStuff() { cout << __FUNCTION__ << "(): has been called" << endl; }
};

bool TestSingleton::alive = false;
TestSingleton* TestSingleton::tst = nullptr;

int main() {
     logger::get_instance();
    // log statements
     logger::get_instance().log(LOG_MODULE::TEXASHOLDEM, LOG_LEVEL::TRACE, "hello this is testing 1");
    logger::get_instance().log(LOG_MODULE::TEXASHOLDEM, LOG_LEVEL::INFO, "hello this is testing 2", 456, 789);
    logger::get_instance() << "hello << operator at work here !!!" << 345456567 << END;
//    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    logger::get_instance().stopLogging();
    g_QProcThread.join();
    return 0;
}

