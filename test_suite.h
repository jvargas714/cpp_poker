#ifndef POKER_TEST_SUITE_H
#define POKER_TEST_SUITE_H
#include "master_poker.h"
#include "logger.h"
#define TEST_CNT 10

// tests
void test_bot_chen_hand( int num_tests );
void test_bot_ctors();
void test_hand_detection( const int& num_tests, bool random=true );

// utility
Cards generate_hand();

#endif //POKER_TEST_SUITE_H
