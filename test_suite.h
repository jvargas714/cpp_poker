#ifndef POKER_TEST_SUITE_H
#define POKER_TEST_SUITE_H
#include "master_poker.h"
#include "logger.h"
#define TEST_CNT 10

class poker_tester
{
public:
    // tests
    // logs results to cout
    poker_tester();
    // logs results ot output file
    poker_tester( std::string output_file );
    poker_tester( std::string output_file, int num_tests );
    ~poker_tester(){}

    // tests
    void test_bot_chen_hand( int num_tests=TEST_CNT );
    void test_bot_ctors();
    void test_hand_detection( int num_tests=TEST_CNT, bool random=true );

// utilities
private:
    int _num_tests;
    logger _lg;
    Cards generate_hand();
};

#endif //POKER_TEST_SUITE_H
