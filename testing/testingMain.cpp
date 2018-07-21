#include <iostream>
#include "test_suite.h"

int main(int argc, char** argv) {
    std::cout << "Google Testing Poker....." << std::endl;
    printf("Running main() from gtest_main.cc\n");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
