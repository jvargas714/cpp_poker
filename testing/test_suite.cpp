#include <iostream>
#include "test_suite.h"

void TexasHoldemTest::SetUp() {
    // test copy constructor
    log() << "Setting up Test game.." << std::endl;
    TexasHoldem g1(25, 50, 2500);
    g1.enterGame("player3", 2500);
    g1.enterGame("player4", 2500);
    g1.enterGame( {{"player5", 2500}, {"player6", 2500}} );

    // performing copy operation
    game = g1;
    std::cout << g1 << std::endl;
    std::cout << game << std::endl;
}
