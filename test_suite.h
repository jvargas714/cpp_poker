#ifndef POKER_TEST_SUITE_H
#define POKER_TEST_SUITE_H
#include "master_poker.h"
#include "include/logger.h"
#define TEST_CNT 10

typedef enum opts
{
	HELP, 		// help menu
	HANDS,      // deal 5 cards have analyzer figure out hand type and value
	CHEN,       // calculate starting hand strength from flop
	COMPARE, 	// two players are dealt hands and their hands are compared
	SIMGAME, 	// after this user specifies number of full betting rounds. players etc....
	CONTINUOUS, // coninuously generate test cases based on user input
	ALL,  		// run all test cases
} OPTS;

typedef struct test_case
{
	int n_tests;
	int n_players;
	OPTS type;
} TEST_CASE;

const std::map<std::string, OPTS> ARG_MAP =
{
		{ "-h", HELP }, 		{ "--help", HELP },
		{ "--hands", HANDS }, 	{ "--chen", CHEN },
		{ "--game", SIMGAME },  { "--continuous", CONTINUOUS },
		{ "--all", ALL }, 		{ "--compare", COMPARE }
};

class poker_tester
{
public:
    // tests
    // logs results to cout
    poker_tester();
    ~poker_tester(){}

    // tests
	void run_tests( const TEST_CASE& );
    void test_bot_chen_hand( int num_tests=TEST_CNT );
    void test_bot_ctors();
	void compare_hands( int num_tests=TEST_CNT, const int& n_players=2 );
	void test_hand_detection( int num_tests=TEST_CNT, bool random=true );
};

// utility functions
Cards generate_hand();
std::string display_hand( Cards& );
#endif //POKER_TEST_SUITE_H
